#include "pilefeamodeler.h"

#include <QList>
#include <QListIterator>
#include <QVector>
#include <QTextStream>
#include <QDateTime>

extern int getTzParam(double phi, double b, double sigV, double pEleLength, double *tult, double *z50);
extern int getQzParam(double phiDegree, double b, double sigV, double G, double *qult, double *z50);
extern int getPyParam(double pyDepth,
                      double gamma,
                      double phiDegree,
                      double b,
                      double pEleLength,
                      int puSwitch,
                      int kSwitch,
                      int gwtSwitch,
                      double *pult,
                      double *y50);

// OpenSees include files
#include <Node.h>
#include <ID.h>
#include <SP_Constraint.h>
#include <MP_Constraint.h>
#include <Domain.h>
#include <StandardStream.h>
#include <LinearCrdTransf3d.h>
#include <DispBeamColumn3d.h>
#include <PySimple1.h>
#include <TzSimple1.h>
#include <QzSimple1.h>
#include <ZeroLength.h>
#include <LegendreBeamIntegration.h>
#include <ElasticSection3d.h>
#include <LinearSeries.h>
#include <NodalLoad.h>
#include <LoadPattern.h>
#include <SimulationInformation.h>

#include <NodeIter.h>
#include <ElementIter.h>
#include <SP_ConstraintIter.h>
#include <MP_ConstraintIter.h>
#include <LoadPatternIter.h>

#include <LoadControl.h>
#include <RCM.h>
#include <PlainNumberer.h>
#include <NewtonRaphson.h>
#include <CTestNormDispIncr.h>
#include <TransformationConstraintHandler.h>
#include <PenaltyConstraintHandler.h>
#include <BandGenLinSOE.h>
#include <BandGenLinLapackSolver.h>
#include <StaticAnalysis.h>
#include <AnalysisModel.h>

#include <soilmat.h>
#include <QDebug>


#define SET_3_NDOF if (nDOFs != 3) { nDOFs = 3; out << "model BasicBuilder -ndm 3 -ndf " << nDOFs << " ;" << endl; }
#define SET_6_NDOF if (nDOFs != 6) { nDOFs = 6; out << "model BasicBuilder -ndm 3 -ndf " << nDOFs << " ;" << endl; }

PileFEAmodeler::PileFEAmodeler()
{
    // set default parameters
    modelState.clear();
    modelState.insert(AnalysisState::solutionAvailable, false);
    modelState.insert(AnalysisState::solutionValid, false);
    modelState.insert(AnalysisState::dataExtracted, false);
    modelState.insert(AnalysisState::meshValid, false);
    modelState.insert(AnalysisState::loadValid, false);
    modelState.insert(AnalysisState::analysisValid, false);

    theDomain = new Domain();

    numLoadedNode = -1;

    motionData = QVector<SoilMotionData>(MAXLAYERS, SoilMotionData());

    /* set default parameters */
    this->setDefaultParameters();

    dumpFEMinput = false;
}

PileFEAmodeler::~PileFEAmodeler()
{
    delete theDomain;
    theDomain = nullptr;
}


void PileFEAmodeler::setDefaultParameters(void)
{
    loadControlType = LoadControlType::ForceControl;

    P    = 50.0;   // lateral force on pile cap
    PV   =  0.00;  // vertical force on pile cap
    PMom =  0.00;  // applied moment on pile cap

    HDisp = 0.0; // prescribed horizontal displacement
    VDisp = 0.0; // prescriber vertical displacement

    surfaceDisp    = 0.00;   // prescribed soil surface displacement
    percentage12   = 1.00;   // percentage of surface displacement at 1st layer interface
    percentage23   = 0.00;   // percentage of surface displacement at 2nd layer interface
    percentageBase = 0.00;   // percentage of surface displacement at base of soil column

    // get parameters
    gwtDepth        = 999.0;  // depth of ground water table below the surface
    numPileElements = -1;
    numPiles        = -1;

    // states
    assumeRigidPileHeadConnection = false;
    useToeResistance    = false;
    puSwitch  = 2;  // Hansen
    kSwitch   = 1;  // API
    gwtSwitch = 1;

    // soil layers and related methods
    QVector<soilLayer> mSoilLayers;

    void setupLayers();

    // temporary variables
    gSoil            = 150000.;

    zCoord  =  0.0;    // z-coordinate of point.  Negative if below the surface
    eleSize = 1.00;    // effective element length for p-y and t-z springs
    sigV    = 1000.0;  // effective stress at p-y and t-z springs

    // lateral resistance
    pult = 999.;
    y50 = 1.00;
    // shaft friction
    tult = 999.;
    z50 = 1.0;
    // toe resistance
    qult = 9999.;
    z50q = 1.0;

    // meshing parameters
    minElementsPerLayer = MIN_ELEMENTS_PER_LAYER;
    maxElementsPerLayer = MAX_ELEMENTS_PER_LAYER;
    numElementsInAir    = NUM_ELEMENTS_IN_AIR;

    numNodePiles = 0
            ;
    for (int k=0; k<MAXPILES;k++)
    {
        pileInfo[k].L1           = 0.00;      // pile length above ground (all the same)
        pileInfo[k].L2           = 10.0;      // embedded length of pile
        pileInfo[k].pileDiameter = 1.00;      // pile diameter
        pileInfo[k].E            = 1.00;      // pile modulus of elasticity
        pileInfo[k].xOffset      = double(k); // x-offset of pile

        pileInfo[k].numNodePile  = 0;
        pileInfo[k].maxLayers    = 0;
        pileInfo[k].nodeIDoffset = 0;
        pileInfo[k].elemIDoffset = 0;
    }

    // pile head parameters
    EI = 1.;
    EA = 1.;
    GJ = 1.0e12;

    /* reset states */
    DISABLE_STATE(AnalysisState::meshValid);
    DISABLE_STATE(AnalysisState::loadValid);
    DISABLE_STATE(AnalysisState::analysisValid);
    DISABLE_STATE(AnalysisState::dataExtracted);
    DISABLE_STATE(AnalysisState::solutionValid);
    DISABLE_STATE(AnalysisState::solutionAvailable);
}

void PileFEAmodeler::updatePiles(QVector<PILE_INFO> &newPileInfo)
{
    numPiles = newPileInfo.size();

    for (int k=0; k<numPiles; k++)
    {
        pileInfo[k].L1           = newPileInfo[k].L1;
        pileInfo[k].L2           = newPileInfo[k].L2;
        pileInfo[k].pileDiameter = newPileInfo[k].pileDiameter;
        pileInfo[k].E            = newPileInfo[k].E;
        pileInfo[k].xOffset      = newPileInfo[k].xOffset;

        pileInfo[k].numNodePile  = 0;
        pileInfo[k].maxLayers    = 0;
        pileInfo[k].nodeIDoffset = 0;
        pileInfo[k].elemIDoffset = 0;
    }

    DISABLE_STATE(AnalysisState::meshValid);
    DISABLE_STATE(AnalysisState::solutionValid);
    DISABLE_STATE(AnalysisState::solutionAvailable);
    DISABLE_STATE(AnalysisState::dataExtracted);
}

void PileFEAmodeler::updateSwitches(bool useToe, bool assumeRigidHead)
{
    if (useToeResistance != useToe)
    {
        useToeResistance = useToe;
        DISABLE_STATE(AnalysisState::meshValid);
        DISABLE_STATE(AnalysisState::solutionValid);
        DISABLE_STATE(AnalysisState::solutionAvailable);
        DISABLE_STATE(AnalysisState::dataExtracted);
    }

    if (assumeRigidPileHeadConnection != assumeRigidHead)
    {
        assumeRigidPileHeadConnection = assumeRigidHead;
        DISABLE_STATE(AnalysisState::meshValid);
        DISABLE_STATE(AnalysisState::solutionValid);
        DISABLE_STATE(AnalysisState::solutionAvailable);
        DISABLE_STATE(AnalysisState::dataExtracted);
    }
}

void PileFEAmodeler::setLoadType(LoadControlType type)
{
    if (loadControlType != type)
    {
        loadControlType = type;
        DISABLE_STATE(AnalysisState::loadValid);
        DISABLE_STATE(AnalysisState::solutionValid);
        DISABLE_STATE(AnalysisState::solutionAvailable);
        DISABLE_STATE(AnalysisState::dataExtracted);
    }
}

void PileFEAmodeler::updateLoad(double Px, double Py, double Moment)
{
    loadControlType = LoadControlType::ForceControl;

    P    = Px;     // lateral force on pile cap
    PV   = Py;     // vertical force on pile cap
    PMom = Moment; // applied moment on pile cap

    DISABLE_STATE(AnalysisState::loadValid);
    DISABLE_STATE(AnalysisState::solutionValid);
    DISABLE_STATE(AnalysisState::solutionAvailable);
    DISABLE_STATE(AnalysisState::dataExtracted);
}

void PileFEAmodeler::updateSoil(QVector<soilLayer> &layers)
{
    mSoilLayers = layers;

    DISABLE_STATE(AnalysisState::meshValid);
    DISABLE_STATE(AnalysisState::loadValid);
    DISABLE_STATE(AnalysisState::solutionValid);
    DISABLE_STATE(AnalysisState::solutionAvailable);
    DISABLE_STATE(AnalysisState::dataExtracted);
}

void PileFEAmodeler::updateGWtable(double depth)
{
    if (gwtDepth == depth) return;

    gwtDepth = depth;
    DISABLE_STATE(AnalysisState::meshValid);
    DISABLE_STATE(AnalysisState::loadValid);
    DISABLE_STATE(AnalysisState::solutionValid);
    DISABLE_STATE(AnalysisState::solutionAvailable);
    DISABLE_STATE(AnalysisState::dataExtracted);
}

void PileFEAmodeler::updateDisplacement(double ux, double uy)
{
    loadControlType = LoadControlType::PushOver;

    HDisp = ux; // prescribed horizontal displacement
    VDisp = uy; // prescriber vertical displacement

    DISABLE_STATE(AnalysisState::loadValid);
    DISABLE_STATE(AnalysisState::solutionValid);
    DISABLE_STATE(AnalysisState::solutionAvailable);
    DISABLE_STATE(AnalysisState::dataExtracted);
}

void PileFEAmodeler::updateDispProfile(QVector<double> &profile)
{
    loadControlType = LoadControlType::SoilMotion;

    surfaceDisp    = 0.00;   // prescribed soil surface displacement
    percentage12   = 1.00;   // percentage of surface displacement at 1st layer interface
    percentage23   = 0.00;   // percentage of surface displacement at 2nd layer interface
    percentageBase = 0.00;   // percentage of surface displacement at base of soil column

    if (profile.size() > 0) surfaceDisp    = profile[0];   // prescribed soil surface displacement
    if (profile.size() > 1) percentage12   = profile[1];   // percentage of surface displacement at 1st layer interface
    if (profile.size() > 2) percentage23   = profile[2];   // percentage of surface displacement at 2nd layer interface
    if (profile.size() > 3) percentageBase = profile[3];   // percentage of surface displacement at base of soil column

    soilMotion[0] = surfaceDisp;
    soilMotion[1] = surfaceDisp*percentage12;
    soilMotion[2] = surfaceDisp*percentage23;
    soilMotion[3] = surfaceDisp*percentageBase;

    DISABLE_STATE(AnalysisState::loadValid);
    DISABLE_STATE(AnalysisState::solutionValid);
    DISABLE_STATE(AnalysisState::solutionAvailable);
    DISABLE_STATE(AnalysisState::dataExtracted);

    this->updateMotionData();
}

void PileFEAmodeler::setAnalysisType(QString)
{
    qWarning() << "PileFEAmodeler::setAnalysisType(QString) not implemented";
    DISABLE_STATE(AnalysisState::analysisValid);
}

bool PileFEAmodeler::doAnalysis()
{
    if (!CHECK_STATE(AnalysisState::meshValid))
    {
        this->buildMesh();
        DISABLE_STATE(AnalysisState::analysisValid);
    }
    if (!CHECK_STATE(AnalysisState::loadValid))
    {
        this->buildLoad();
        DISABLE_STATE(AnalysisState::analysisValid);
    }
    if (!CHECK_STATE(AnalysisState::analysisValid))
    {
        DISABLE_STATE(AnalysisState::dataExtracted);
        DISABLE_STATE(AnalysisState::solutionValid);
        this->buildAnalysis();
    }

    DISABLE_STATE(AnalysisState::solutionAvailable);

    //
    //analyze & get results
    //
    int converged = theAnalysis->analyze(20);
    theDomain->calculateNodalReactions(0);

    // the solution exists, but it may or may not be valid !
    ENABLE_STATE(AnalysisState::solutionAvailable);

    bool isConverged;

    if (converged < 0)
    {
        DISABLE_STATE(AnalysisState::solutionValid);
        isConverged = false;
    }
    else
    {
        ENABLE_STATE(AnalysisState::solutionValid);
        isConverged = true;
    }

    return isConverged;
}

void PileFEAmodeler::buildMesh()
{
    if (CHECK_STATE(AnalysisState::meshValid)) return;

    QTextStream out(FEMfile);
    int materialIndex = 0;
    int nDOFs = 0;
    excentricity = 0.0;

    // clear the list of soil nodes == those nodes where p-y springs are attaching to the soil
    soilNodes.clear();

    for (int k=0; k<MAXPILES; k++) {
        headNodeList[k] = {-1, -1, 0.0, 1.0, 1.0};
    }

    if (dumpFEMinput)
    {
        out << "#----------------------------------------------------------"  << endl;
        out << "#   file created by PileGroup Tool                         "  << endl;
        out << "#                                                          "  << endl;
        out << "#   author:      Peter Mackenzie-Helnwein                  "  << endl;
        out << "#   affiliation: University of Washington                  "  << endl;
        out << "#   created on:  " << QDateTime::currentDateTime().toString() << endl;
        out << "#----------------------------------------------------------"  << endl;
        out << endl;
    }

    // clear existing model
    theDomain->clearAll();
    OPS_clearAllUniaxialMaterial();
    ops_Dt = 0.0;

    capNodeList.clear();

    //
    // find meshing parameters
    //

    QVector<QVector<int>> elemsInLayer(MAXPILES,QVector<int>(3,minElementsPerLayer));
    depthOfLayer = QVector<double>(4, 0.0); // add a buffer element for bottom of the third layer

    numNodePiles = numPiles;

    for (int k=0; k<MAXPILES; k++) {
        pileInfo[k].numNodePile  = 0;
        pileInfo[k].maxLayers    = MAXLAYERS;
        pileInfo[k].nodeIDoffset = 0;
        pileInfo[k].elemIDoffset = 0;
        pileInfo[k].firstNodeTag = -1;    // for error detection
        pileInfo[k].lastNodeTag  = -1;    // for error detection
        pileInfo[k].firstElementTag = -1; // for error detection
        pileInfo[k].lastElementTag  = -1; // for error detection
    }

    /* ******** sizing and adjustments ******** */

    for (int pileIdx=0; pileIdx<numPiles; pileIdx++)
    {
        pileInfo[pileIdx].numNodePile = 2; // bottom plus surface node
        if (pileInfo[pileIdx].L1 > 0.0001) pileInfo[pileIdx].numNodePile += numElementsInAir; // free standing

        //
        // find active layers for pile #pileIdx
        //
        for (int iLayer=0; iLayer < pileInfo[pileIdx].maxLayers; iLayer++)
        {
            if (depthOfLayer[iLayer] >= pileInfo[pileIdx].L2) {
                // this pile only penetrates till layer #iLayer
                pileInfo[pileIdx].maxLayers = iLayer;
                break;
            }

            double thickness = mSoilLayers[iLayer].getLayerThickness();

            // compute bottom of this layer/top of the next layer
            if (depthOfLayer[iLayer] + thickness < pileInfo[pileIdx].L2 && iLayer == MAXLAYERS - 1) {
                thickness = pileInfo[pileIdx].L2 - depthOfLayer[iLayer];
                mSoilLayers[iLayer].setLayerThickness(thickness);
            }
            depthOfLayer[iLayer+1] = depthOfLayer[iLayer] + thickness;

            // check if layer ends below the pile toe and adjust thickness for mesh generation
            if (depthOfLayer[iLayer+1] > pileInfo[pileIdx].L2) {
                thickness = pileInfo[pileIdx].L2 - depthOfLayer[iLayer];
            }

            int numElemThisLayer = int(thickness/pileInfo[pileIdx].pileDiameter);
            if (numElemThisLayer < minElementsPerLayer) numElemThisLayer = minElementsPerLayer;
            if (numElemThisLayer > maxElementsPerLayer) numElemThisLayer = maxElementsPerLayer;

            // remember number of elements in this layer
            elemsInLayer[pileIdx][iLayer] = numElemThisLayer;

            // total node count
            pileInfo[pileIdx].numNodePile += numElemThisLayer;

            // update layer information on overburdon stress and water table
            double overburdonStress;
            if (iLayer > 0) {
                overburdonStress = mSoilLayers[iLayer-1].getLayerBottomStress();
            }
            else {
                overburdonStress = 0.00;
            }
            mSoilLayers[iLayer].setLayerOverburdenStress(overburdonStress);

            double groundWaterHead = gwtDepth - depthOfLayer[iLayer];
            mSoilLayers[iLayer].setLayerGWHead(groundWaterHead);
        }

        // add numper of nodes in this pile to global number of nodes
        numNodePiles += pileInfo[pileIdx].numNodePile;
    }

    /* ******** done with sizing and adjustments ******** */

    locList.clear();
    pultList.clear();
    y50List.clear();
    tultList.clear();
    z50List.clear();

    for (int pileIdx=0; pileIdx<MAXPILES; pileIdx++)
    {
        locList.append(new QVector<double>(pileInfo[pileIdx].numNodePile));
        pultList.append(new QVector<double>(pileInfo[pileIdx].numNodePile));
        y50List.append(new QVector<double>(pileInfo[pileIdx].numNodePile));
        tultList.append(new QVector<double>(pileInfo[pileIdx].numNodePile));
        z50List.append(new QVector<double>(pileInfo[pileIdx].numNodePile));
    }

    int ioffset  = numNodePiles;              // for p-y spring nodes
    int ioffset2 = ioffset + numNodePiles;    // for pile nodes
    int ioffset3 = ioffset2 + numNodePiles;   // for t-z spring nodes
    int ioffset4 = ioffset3 + numNodePiles;   // for toe resistance nodes
    int ioffset5 = ioffset4 + numNodePiles;   // for pile cap nodes

    /* ******** build the finite element mesh ******** */

    if (dumpFEMinput)
    {
        out << endl;
        out << "#----------------------------------------------------------" << endl;
        out << "#   creating the FE model                                  " << endl;
        out << "#----------------------------------------------------------" << endl;
        SET_3_NDOF
    }

    //
    // matrix used to constrain spring and pile nodes with equalDOF (identity constraints)
    //
    static Matrix Ccr (2, 2);
    Ccr.Zero(); Ccr(0,0)=1.0; Ccr(1,1)=1.0;
    static ID rcDof (2);
    rcDof(0) = 0;
    rcDof(1) = 2;

    // create the vectors for the spring elements orientation
    static Vector x(3); x(0) = 1.0; x(1) = 0.0; x(2) = 0.0;
    static Vector y(3); y(0) = 0.0; y(1) = 1.0; y(2) = 0.0;

    // direction for spring elements
    ID direction(2);
    direction[0] = 0;
    direction[1] = 2;

    // initialization of counters
    int numNode = 0;
    int numElem = ioffset2;
    int nodeTag = 0;

    pileInfo[0].nodeIDoffset = ioffset2;
    pileInfo[0].elemIDoffset = ioffset2;
    for (int k=1; k<numPiles; k++) {
        pileInfo[k].nodeIDoffset = pileInfo[k-1].nodeIDoffset + pileInfo[k-1].numNodePile;
        pileInfo[k].elemIDoffset = pileInfo[k-1].elemIDoffset + pileInfo[k-1].numNodePile - 1;
    }

    // build the FE-model one pile at a time

    for (int pileIdx=0; pileIdx<numPiles; pileIdx++)
    {
        if (dumpFEMinput)
        {
            out << endl;
            out << endl;
            out << "##   SOIL SPRINGS FOR PILE " << pileIdx+1 << endl;
            out << endl;
        }

        //
        // compute pile properties (compute once; used for all pile elements)
        //
        double PI = 3.14159;
        double A  = 0.2500 * PI * pileInfo[pileIdx].pileDiameter * pileInfo[pileIdx].pileDiameter;
        double Iz = 0.0625 *  A * pileInfo[pileIdx].pileDiameter * pileInfo[pileIdx].pileDiameter;
        double G  = pileInfo[pileIdx].E/(2.0*(1.+0.3));
        double J  = 1.0e10;

        // suitable pile head parameters (make pile head stiff)
        if (100.*pileInfo[pileIdx].E*A > EA ) EA = 100.*pileInfo[pileIdx].E*A;
        if (100.*pileInfo[pileIdx].E*Iz > EI) EI = 100.*pileInfo[pileIdx].E*Iz;
        if (10.*G*J > GJ)            GJ = 10.*G*J;

        if (dumpFEMinput)
        {
            // section Elastic $secTag $E $A $Iz $Iy $G $J <$alphaY $alphaZ>
            out << "section Elastic " << pileIdx+1 << " " // $secTag
                                      << pileInfo[pileIdx].E   << " "     // $E
                                      << A   << " "     // $A
                                      << Iz  << " "     // $Iz
                                      << Iz  << " "     // $Iy
                                      << G   << " "     // $G
                                      << J   << " ;"     // $J
                                      << endl;
        }

        //
        // Ready to generate the structure
        //

        /* embedded pile portion */

        zCoord = -pileInfo[pileIdx].L2;

        //
        // create bottom pile node
        //
        numNode += 1;

        Node *theNode = 0;
        nodeTag = numNode+ioffset2;

        theNode = new Node(nodeTag, 6, pileInfo[pileIdx].xOffset, 0., zCoord);  theDomain->addNode(theNode);
        pileInfo[pileIdx].firstNodeTag = nodeTag;

        if (dumpFEMinput)
        {
            SET_6_NDOF
            out << "node " << nodeTag << " " << pileInfo[pileIdx].xOffset << " 0. " << zCoord << " ;" << endl;
        }

        if (numNode != 1) {
            SP_Constraint *theSP = 0;
            theSP = new SP_Constraint(nodeTag, 1, 0., true); theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(nodeTag, 3, 0., true); theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(nodeTag, 5, 0., true); theDomain->addSP_Constraint(theSP);

            if (dumpFEMinput) { out << "fix  " << nodeTag << "  0 1 0 1 0 1 ;" << endl; }
        }

        //
        // add toe resistance (if requested)
        //

        if (useToeResistance) {
            Node *theNode = 0;

            theNode = new Node(numNode,         3, pileInfo[pileIdx].xOffset, 0., zCoord);  theDomain->addNode(theNode);
            theNode = new Node(numNode+ioffset, 3, pileInfo[pileIdx].xOffset, 0., zCoord);  theDomain->addNode(theNode);

            if (dumpFEMinput)
            {
                SET_3_NDOF
                out << "node " << numNode         << " " << pileInfo[pileIdx].xOffset << " 0. " << zCoord << " ;" << endl;
                out << "node " << numNode+ioffset << " " << pileInfo[pileIdx].xOffset << " 0. " << zCoord << " ;" << endl;
            }

            SP_Constraint *theSP = 0;
            theSP = new SP_Constraint(numNode, 0, 0., true);  theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode, 1, 0., true);  theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode, 2, 0., true);  theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode+ioffset, 0, 0., true);  theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode+ioffset, 1, 0., true);  theDomain->addSP_Constraint(theSP);

            if (dumpFEMinput)
            {
                out << "fix  " << numNode         << "  1 1 1 ;" << endl;
                out << "fix  " << numNode+ioffset << "  1 1 0 ;" << endl;
            }

            MP_Constraint *theMP = new MP_Constraint(numNode+ioffset2, numNode+ioffset, Ccr, rcDof, rcDof);
            theDomain->addMP_Constraint(theMP);

            if (dumpFEMinput)
            {
                out << "equalDOF " << numNode+ioffset2 << " " << numNode+ioffset << " ";
                for (int k=0; k<rcDof.Size(); k++) { out << rcDof(k)+1 << " "; }
                out << " ;" << endl;
            }

            // # q-z spring material
            // # vertical effective stress at pile tip, no water table (depth is embedded pile length)
            double sigVq  = mSoilLayers[pileInfo[pileIdx].maxLayers-1].getLayerBottomStress();
            double phi  = mSoilLayers[pileInfo[pileIdx].maxLayers-1].getLayerFrictionAng();

            getQzParam(phi, pileInfo[pileIdx].pileDiameter,  sigVq,  gSoil, &qult, &z50q);
            UniaxialMaterial *theMat = new QzSimple1(numNode, 2, qult, z50q, 0.0, 0.0);
            OPS_addUniaxialMaterial(theMat);

            ID Onedirection(1); Onedirection[0] = 2;

            // pile toe
            Element *theEle = new ZeroLength(1+pileIdx+ioffset4, 3, numNode, numNode+ioffset, x, y, 1, &theMat, Onedirection);
            theDomain->addElement(theEle);

            if (dumpFEMinput)
            {
                out << "uniaxialMaterial QzSimple1 " << numNode << " "
                    << 2 << " " << qult << " " << z50q << " 0.0 0.0 ;" << endl;

                out << "element zeroLength "
                    << 1+pileIdx+ioffset4 << " " << numNode << " " << numNode+ioffset
                    << " -mat " << numNode
                    << " -dir ";
                for (int k=0; k<Onedirection.Size(); k++) { out << Onedirection(k)+1 << " "; }
                out << " -orient ";
                for (int k=0; k<x.Size(); k++) { out << x(k) << " "; }
                for (int k=0; k<y.Size(); k++) { out << y(k) << " "; }
                out << " ;" << endl;
            }
        }
        else
        {
            // hold the pile through friction only: t-z springs will do the job
        }

        (*locList[pileIdx])[numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset-1]  = zCoord;
        (*pultList[pileIdx])[numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset-1] = 0.001;
        (*y50List[pileIdx])[numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset-1]  = 0.00001;
        (*tultList[pileIdx])[numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset-1] = 0.001;
        (*z50List[pileIdx])[numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset-1]  = 0.00001;

        //
        // work the way up layer by layer
        //

        for (int iLayer=pileInfo[pileIdx].maxLayers-1; iLayer >= 0; iLayer--)
        {
            double thickness = mSoilLayers[iLayer].getLayerThickness();
            if (depthOfLayer[iLayer+1] > pileInfo[pileIdx].L2) {
                thickness = pileInfo[pileIdx].L2 - depthOfLayer[iLayer];
            }

            eleSize = thickness/(1.0*elemsInLayer[pileIdx][iLayer]);
            int numNodesLayer = elemsInLayer[pileIdx][iLayer] + 1;

            //
            // create spring nodes
            //
            zCoord += 0.5*eleSize;

            for (int i=1; i<numNodesLayer; i++) {
                numNode += 1;

                //
                // spring nodes
                //

                Node *theNode = 0;

                theNode = new Node(numNode,         3, pileInfo[pileIdx].xOffset, 0., zCoord);  theDomain->addNode(theNode);
                theNode = new Node(numNode+ioffset, 3, pileInfo[pileIdx].xOffset, 0., zCoord);  theDomain->addNode(theNode);

                if (dumpFEMinput)
                {
                    SET_3_NDOF
                    out << "node " << numNode         << " " << pileInfo[pileIdx].xOffset << " 0. " << zCoord << " ;" << endl;
                    out << "node " << numNode+ioffset << " " << pileInfo[pileIdx].xOffset << " 0. " << zCoord << " ;" << endl;
                }


                SP_Constraint *theSP = 0;
                //
                // this is the node connected to the soil -- needs to feel the moving soil
                //
                theSP = new SP_Constraint(numNode, 0, 0., true);  theDomain->addSP_Constraint(theSP);
                theSP = new SP_Constraint(numNode, 1, 0., true);  theDomain->addSP_Constraint(theSP);
                theSP = new SP_Constraint(numNode, 2, 0., true);  theDomain->addSP_Constraint(theSP);

                soilNodes.append(SoilNodeData(numNode, -zCoord));

                if (dumpFEMinput)
                {
                    out << "fix  " << numNode         << "  1 1 1 ;" << endl;
                }
                //
                // this is the node connecting to the pile.
                // -- We need to fix the out of plane movement here for the MP constraint won't link that direction
                //
                theSP = new SP_Constraint(numNode+ioffset, 1, 0., true);  theDomain->addSP_Constraint(theSP);

                if (dumpFEMinput)
                {
                    out << "fix  " << numNode+ioffset << "  0 1 0 ;" << endl;
                }

                //
                // pile nodes
                //

                nodeTag = numNode+ioffset2;

                theNode = new Node(nodeTag, 6, pileInfo[pileIdx].xOffset, 0., zCoord);  theDomain->addNode(theNode);

                if (dumpFEMinput)
                {
                    SET_6_NDOF
                    out << "node " << nodeTag <<  "  " << pileInfo[pileIdx].xOffset << " 0. " << zCoord << " ;" << endl;
                }

                if (numNode != 1) {
                    SP_Constraint *theSP = 0;
                    theSP = new SP_Constraint(nodeTag, 1, 0., true); theDomain->addSP_Constraint(theSP);
                    theSP = new SP_Constraint(nodeTag, 3, 0., true); theDomain->addSP_Constraint(theSP);
                    theSP = new SP_Constraint(nodeTag, 5, 0., true); theDomain->addSP_Constraint(theSP);

                    if (dumpFEMinput)
                    {
                        out << "fix  " << nodeTag << "  0 1 0 1 0 1 ;" << endl;
                    }
                }

                // constrain spring and pile nodes with equalDOF (identity constraints)
                MP_Constraint *theMP = new MP_Constraint(numNode+ioffset2, numNode+ioffset, Ccr, rcDof, rcDof);
                theDomain->addMP_Constraint(theMP);

                if (dumpFEMinput)
                {
                    out << "equalDOF " << numNode+ioffset2 << " " << numNode+ioffset << " ";
                    for (int k=0; k<rcDof.Size(); k++) { out << rcDof(k)+1 << " "; }
                    out << " ;" << endl;
                }

                //
                // create soil-spring materials
                //

                // # p-y spring material
                puSwitch  = 2;  // Hanson
                //puSwitch  = 1;  // API // temporary switch
                kSwitch   = 1;  // API

                gwtSwitch = (gwtDepth > -zCoord)?1:2;

                double depthInLayer = -zCoord - depthOfLayer[iLayer];
                sigV = mSoilLayers[iLayer].getEffectiveStress(depthInLayer);
                double phi  = mSoilLayers[iLayer].getLayerFrictionAng();

                UniaxialMaterial *theMat;
                getPyParam(-zCoord, sigV, phi, pileInfo[pileIdx].pileDiameter, eleSize, puSwitch, kSwitch, gwtSwitch, &pult, &y50);

                if(pult <= 0.0 || y50 <= 0.0) {
                    qDebug() << "WARNING -- only accepts positive nonzero pult and y50";
                    qDebug() << "*** iLayer: " << iLayer << "   pile number" << pileIdx+1
                             << "   depth: " << -zCoord
                             << "   depth in layer: " << depthInLayer
                             << "   sigV: "  << sigV
                             << "   diameter: " << pileInfo[pileIdx].pileDiameter << "   eleSize: " << eleSize;
                    qDebug() << "*** pult: " << pult << "   y50: " << y50;
                }

                theMat = new PySimple1(numNode, 0, 2, pult, y50, 0.0, 0.0);
                OPS_addUniaxialMaterial(theMat);

                if (dumpFEMinput)
                {
                    out << "uniaxialMaterial PySimple1 " << numNode << " 2 " << pult << " " << y50 << " 0.0" << " ;" << endl;
                }

                // t-z spring material
                getTzParam(phi, pileInfo[pileIdx].pileDiameter,  sigV,  eleSize, &tult, &z50);

                if (tult <= 0.0 || z50 <= 0.0) {
                    qDebug() << "WARNING -- only accepts positive nonzero tult and z50";
                    qDebug() << "*** iLayer: " << iLayer << "   pile number" << pileIdx+1
                             << "   depth: " << -zCoord << "   sigV: "  << sigV
                             << "   diameter: " << pileInfo[pileIdx].pileDiameter << "   eleSize: " << eleSize;
                    qDebug() << "*** tult: " << tult << "   z50: " << z50;
                }

                theMat = new TzSimple1(numNode+ioffset, 0, 2, tult, z50, 0.0);
                OPS_addUniaxialMaterial(theMat);

                if (dumpFEMinput)
                {
                    out << "uniaxialMaterial TzSimple1 " << numNode+ioffset << " 2 " << tult << " " << z50 << " 0.0 ;" << endl;
                }

                //
                // collect plot data
                //

                (*locList[pileIdx])[numNode+ioffset2- pileInfo[pileIdx].nodeIDoffset-1]  = zCoord;
                // pult is a nodal value for the p-y spring.
                // It needs to be scaled by element length ito represent a line load
                (*pultList[pileIdx])[numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset-1] = pult/eleSize;
                (*y50List[pileIdx])[numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset-1]  = y50;
                (*tultList[pileIdx])[numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset-1] = tult/eleSize;
                (*z50List[pileIdx])[numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset-1]  = z50;

                //
                // create soil spring elements
                //

                UniaxialMaterial *theMaterials[2];
                theMaterials[0] = OPS_getUniaxialMaterial(numNode);
                theMaterials[1] = OPS_getUniaxialMaterial(numNode+ioffset);
                Element *theEle = new ZeroLength(numNode+ioffset3, 3, numNode, numNode+ioffset, x, y, 2, theMaterials, direction);
                theDomain->addElement(theEle);

                if (dumpFEMinput)
                {
                    out << "element zeroLength "
                        << numNode+ioffset3 << " " << numNode << " " << numNode+ioffset
                        << " -mat " << numNode << " " << numNode+ioffset
                        << " -dir ";
                    for (int k=0; k<direction.Size(); k++) { out << direction(k)+1 << " "; }
                    out << " -orient ";
                    for (int k=0; k<x.Size(); k++) { out << x(k) << " "; }
                    for (int k=0; k<y.Size(); k++) { out << y(k) << " "; }
                    out << " ;" << endl;
                }

                zCoord += eleSize;
            }
            // back to the layer interface
            zCoord -= 0.5*eleSize;

            //qDebug() << "Layer interface at " << "zCoord: " << zCoord << ", eleSize: " << eleSize;

        } // on to the next layer, working our way up from the bottom

        if (ABS(zCoord) > 1.0e-2) {
            qDebug() << "ERROR in node generation: surface reached at " << zCoord << endln;
        }

        //
        // add elements above ground
        //

        if (pileInfo[pileIdx].L1 > 0.0001) {
            eleSize = pileInfo[pileIdx].L1 / (1.0*numElementsInAir);
            zCoord = 0.0;
        }
        else {
            eleSize = 999.99;
            zCoord = 0.0;
        }

        while (zCoord < (pileInfo[pileIdx].L1 + 1.0e-6)) {
            numNode += 1;

            nodeTag = numNode+ioffset2;

            Node *theNode = new Node(nodeTag, 6, pileInfo[pileIdx].xOffset, 0., zCoord);  theDomain->addNode(theNode);

            if (dumpFEMinput)
            {
                SET_6_NDOF
                out << "node " << nodeTag << "   " << pileInfo[pileIdx].xOffset << " 0. " << zCoord << " ;" << endl;
            }

            if (numNode != 1) {
                SP_Constraint *theSP = 0;
                theSP = new SP_Constraint(nodeTag, 1, 0., true); theDomain->addSP_Constraint(theSP);
                theSP = new SP_Constraint(nodeTag, 3, 0., true); theDomain->addSP_Constraint(theSP);
                theSP = new SP_Constraint(nodeTag, 5, 0., true); theDomain->addSP_Constraint(theSP);

                if (dumpFEMinput)
                {
                    out << "fix " << nodeTag << "   0 1 0 1 0 1 ;" << endl;
                }

            }

            (*locList[pileIdx])[numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset-1]  = zCoord;
            (*pultList[pileIdx])[numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset-1] = 0.001;
            (*y50List[pileIdx])[numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset-1]  = 0.00001;
            (*tultList[pileIdx])[numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset-1] = 0.001;
            (*z50List[pileIdx])[numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset-1]  = 0.00001;

            zCoord += eleSize;
        }


        headNodeList[pileIdx] = {pileIdx, nodeTag, pileInfo[pileIdx].xOffset};
        pileInfo[pileIdx].lastNodeTag = nodeTag;

        //
        // create pile elements
        //

        static Vector crdV(3); crdV(0)=0.; crdV(1)=-1; crdV(2) = 0.;
        CrdTransf *theTransformation = new LinearCrdTransf3d(1, crdV);

        if (dumpFEMinput)
        {
            out << endl;
            out << endl;
            out << "## BUILDING PILE " << pileIdx+1 << endl;
            out << endl;
            out << "# create coordinate-transformation object" << endl;
            out << "geomTransf Linear  " << pileIdx+1;
            for (int k=0; k<crdV.Size(); k++) { out << " " << crdV(k); }
            out << " ;" << endl;
        }

        pileInfo[pileIdx].firstElementTag = numElem+1;

        for (int i=0; i<pileInfo[pileIdx].numNodePile-1; i++) {
            numElem++;
            BeamIntegration *theIntegration = new LegendreBeamIntegration();
            SectionForceDeformation *theSections[3];
            SectionForceDeformation *theSection = new ElasticSection3d(pileIdx+1, pileInfo[pileIdx].E, A, Iz, Iz, G, J);
            theSections[0] = theSection;
            theSections[1] = theSection;
            theSections[2] = theSection;

            Element *theEle = new DispBeamColumn3d(numElem,
                                                   pileInfo[pileIdx].nodeIDoffset+i+1,
                                                   pileInfo[pileIdx].nodeIDoffset+i+2,
                                                   3, theSections, *theIntegration, *theTransformation);
            theDomain->addElement(theEle);

            if (dumpFEMinput)
            {
                // need a section definition

                //element dispBeamColumn $eleTag $iNode $jNode $numIntgrPts $secTag $transfTag
                //                             <-mass $massDens> <-cMass> <-integration $intType>
                out << "element dispBeamColumn " << numElem << " "
                        << pileInfo[pileIdx].nodeIDoffset+i+1 << " "
                        << pileInfo[pileIdx].nodeIDoffset+i+2 << " "
                        << 3 << " "
                        << pileIdx+1 << " "   // section tag: use pileIdx+1
                        << pileIdx+1 << " "   // transformation tag: use pileIdx+1
                        << " ;" << endl;
            }
        }

        pileInfo[pileIdx].lastElementTag = numElem;
    }

    //
    // *** construct the pile cap ***
    //
    if (numPiles > 0) {

        // sort piles by xOffset

        for (int i=0; i<numPiles; i++) {
            for (int j=0; j<numPiles-i; j++) {
                if (headNodeList[i].x < headNodeList[j].x) SWAP(headNodeList[i],headNodeList[j]);
            }
        }

        excentricity = 0.50*(headNodeList[numPiles-1].x - headNodeList[0].x);

        // set up transformation and orientation for the pile cap elements

        static Vector crdV(3); crdV(0)=0.; crdV(1)=-1; crdV(2) = 0.;
        CrdTransf *theTransformation = new LinearCrdTransf3d(1, crdV);

        if (dumpFEMinput)
        {
            out << endl;
            out << endl;
            out << "## BUILDING PILE CAP" << endl;
            out << endl;
            out << "# create coordinate-transformation object" << endl;
            out << "geomTransf Linear  " << MAXPILES+1;
            for (int k=0; k<crdV.Size(); k++) { out << " " << crdV(k); }
            out << " ;" << endl;
        }

        // define beam element integration and cross sections

        BeamIntegration *theIntegration = new LegendreBeamIntegration();
        SectionForceDeformation *theSections[3];
        SectionForceDeformation *theSection = new ElasticSection3d(numPiles+1, 1., EA, EI, EI, 1., GJ);
        theSections[0] = theSection;
        theSections[1] = theSection;
        theSections[2] = theSection;

        if (dumpFEMinput)
        {
            // section Elastic $secTag $E $A $Iz $Iy $G $J <$alphaY $alphaZ>
            out << "section Elastic " << MAXPILES+1 << " " // $secTag
                                      << 1.0 << " "     // $E
                                      << EA  << " "     // $A
                                      << EI  << " "     // $Iz
                                      << EI  << " "     // $Iy
                                      << 1.0 << " "     // $G
                                      << GJ  << " "     // $J
                                      << " ;" << endl;
        }

        int prevNode = -1;

        for (int pileIdx=0; pileIdx<numPiles; pileIdx++) {

            // create node for pile head
            numNode++;
            int nodeTag = numNode + ioffset5;

            Node *theNode = new Node(nodeTag, 6, headNodeList[pileIdx].x, 0., pileInfo[pileIdx].L1);  theDomain->addNode(theNode);
            capNodeList.append({pileIdx, nodeTag, headNodeList[pileIdx].x, pileInfo[pileIdx].L1});

            if (dumpFEMinput)
            {
                SET_6_NDOF
                out << "node " << nodeTag << " " << headNodeList[pileIdx].x << " " << 0.0 << " " << pileInfo[pileIdx].L1 << " ;" << endl;
            }

            // create single point constraints
            if (assumeRigidPileHeadConnection) {
                // constrain spring and pile nodes with equalDOF (identity constraints)
                static Matrix Chr (6, 6);
                Chr.Zero();
                Chr(0,0)=1.0; Chr(1,1)=1.0; Chr(2,2)=1.0; Chr(3,3)=1.0; Chr(4,4)=1.0; Chr(5,5)=1.0;
                static ID hcDof (6);
                hcDof(0) = 0; hcDof(1) = 1; hcDof(2) = 2; hcDof(3) = 3; hcDof(4) = 4; hcDof(5) = 5;

                //qDebug() << "MP_Constraint(" << nodeTag << "," << headNodeList[pileIdx].nodeIdx << "," << "Idty(6,6)" << "," << "{0,1,2,3,4,5}" << "," << "{0,1,2,3,4,5}" << ")";

                MP_Constraint *theMP = new MP_Constraint(nodeTag, headNodeList[pileIdx].nodeIdx, Chr, hcDof, hcDof);
                theDomain->addMP_Constraint(theMP);

                if (dumpFEMinput)
                {
                    out << "equalDOF " << nodeTag << " " << headNodeList[pileIdx].nodeIdx << " ";
                    for (int k=0; k<hcDof.Size(); k++) { out << hcDof(k)+1 << " "; }
                    out << " ;" << endl;
                }
            }
            else {
                // constrain spring and pile nodes with equalDOF (identity constraints)
                static Matrix Chl (5, 5);
                Chl.Zero();
                Chl(0,0)=1.0; Chl(1,1)=1.0; Chl(2,2)=1.0; Chl(3,3)=1.0; Chl(4,4)=1.0;
                static ID hlDof (5);
                hlDof(0) = 0; hlDof(1) = 1; hlDof(2) = 2; hlDof(3) = 3; hlDof(4) = 5;

                //qDebug() << "MP_Constraint(" << nodeTag << "," << headNodeList[pileIdx].nodeIdx << "," << "Idty(5,5)" << "," << "{0,1,2,3,5}" << "," << "{0,1,2,3,5}" << ")";

                MP_Constraint *theMP = new MP_Constraint(nodeTag, headNodeList[pileIdx].nodeIdx, Chl, hlDof, hlDof);
                theDomain->addMP_Constraint(theMP);

                if (dumpFEMinput)
                {
                    out << "equalDOF " << nodeTag << " " << headNodeList[pileIdx].nodeIdx << " ";
                    for (int k=0; k<hlDof.Size(); k++) { out << hlDof(k)+1 << " "; }
                    out << " ;" << endl;
                }
            }

            // create beams for pile head
            if (prevNode > 0) {
                numElem++;

                Element *theEle = new DispBeamColumn3d(numElem, prevNode, nodeTag,
                                                       3, theSections, *theIntegration, *theTransformation);
                theDomain->addElement(theEle);

                if (dumpFEMinput)
                {
                    // need a section definition

                    //element dispBeamColumn $eleTag $iNode $jNode $numIntgrPts $secTag $transfTag
                    //                             <-mass $massDens> <-cMass> <-integration $intType>
                    out << "element dispBeamColumn " << numElem << " "
                            << prevNode << " "
                            << nodeTag << " "
                            << 3 << " "
                            << MAXPILES+1 << " "   // section tag: use pileIdx+1
                            << MAXPILES+1 << " "   // transformation tag: use pileIdx+1
                            << " ;" << endl;
                }
            }

            prevNode = nodeTag;
        }
    }

    if (numPiles == 1) {
        /* need extra fixities to prevent singular system matrix */
        int nodeTag = numNode + ioffset5;

        SP_Constraint *theSP = 0;
        theSP = new SP_Constraint(nodeTag, 4, 0., true); theDomain->addSP_Constraint(theSP);

        if (dumpFEMinput)
        {
            out << "fix  " << nodeTag << "  0 0 0 0 1 0 ;" << endl;
        }
    }

    //numLoadedNode = headNodeList[0].nodeIdx;
    numLoadedNode = capNodeList.first().nodeIdx;

    /* *** done with the pile head *** */

    if (numNode != numNodePiles) {
        qDebug() << "ERROR: " << numNode << " nodes generated but " << numNodePiles << "expected" << endln;
    }

    if (dumpFEMinput)
    {
        out << endl;
        out << "#----------------------------------------------------------" << endl;
        out << "#   create recorders                                       " << endl;
        out << "#----------------------------------------------------------" << endl;
        out << endl;
        out << "# node recorder for pile cap" << endl;
        out << "recorder Node -file PileCap.txt -time -node " << numLoadedNode << " -dof 1 3 5 disp" << endl;
        out << "recorder Node -file Pile1_Disp.txt -time "
            << " -nodeRange " << pileInfo[0].firstNodeTag << " " << pileInfo[0].lastNodeTag
            << " -dof 1 3 5 disp" << endl;
        out << endl;
        out << "# element recorder" << endl;
        out << "recorder Element -file Pile1_Forces.txt -time "
            << " -eleRange " << pileInfo[0].firstElementTag << " " << pileInfo[0].lastElementTag
            << " -dT 9.95 forces" << endl;
        out << endl;
    }

    ENABLE_STATE(AnalysisState::meshValid);
    DISABLE_STATE(AnalysisState::solutionAvailable);
}

void PileFEAmodeler::buildLoad()
{
    QTextStream out(FEMfile);

    //
    // create load pattern and add loads
    //

    if (dumpFEMinput)
    {
        out << endl;
        out << "#----------------------------------------------------------"  << endl;
        out << "#   create load pattern                                    "  << endl;
        out << "#----------------------------------------------------------"  << endl;
        out << endl;
    }

    LinearSeries *theTimeSeries  = nullptr;
    LoadPattern  *theLoadPattern = nullptr;
    NodalLoad    *theLoad        = nullptr;

    theTimeSeries  = new LinearSeries(1, 1.0);
    theLoadPattern = new LoadPattern(1);
    theLoadPattern->setTimeSeries(theTimeSeries);

    static Vector load(6);

    switch (loadControlType)
    {
    case LoadControlType::ForceControl:

        if (dumpFEMinput)
        {
            out << "puts \"Running force control...\" ;"  << endl;
            out << "# create load pattern:" << endl;
            out << "pattern Plain 200 Linear {"    << endl;
        }

        // numLoadedNode is the ID of the reference node that will be pushed
        load.Zero();
        load(0) = P;
        load(2) = PV;
        load(4) = PMom - PV*excentricity;

        if (numLoadedNode >= 0)
        {
            theLoad = new NodalLoad(0, numLoadedNode, load);
            theLoadPattern->addNodalLoad(theLoad);
            theDomain->addLoadPattern(theLoadPattern);

            if (dumpFEMinput)
            {
                out << "              load " << numLoadedNode;
                for (int k=0;k<load.Size();k++) { out << " " << load(k);};
                out << " ;" << endl;
            }

            ENABLE_STATE(AnalysisState::loadValid);
            DISABLE_STATE(AnalysisState::solutionAvailable);
            DISABLE_STATE(AnalysisState::solutionValid);
        };

        if (dumpFEMinput) { out << "          } ;" << endl;  out << endl; }

        break;

    case LoadControlType::PushOver:

        // numLoadedNode is the ID of the reference node that will be pushed
        if (numLoadedNode > 0)
        {
            theLoadPattern->addSP_Constraint(new SP_Constraint(numLoadedNode, 0, HDisp, false));
            theLoadPattern->addSP_Constraint(new SP_Constraint(numLoadedNode, 2, VDisp, false));

            theDomain->addLoadPattern(theLoadPattern);

            ENABLE_STATE(AnalysisState::loadValid);
            DISABLE_STATE(AnalysisState::solutionAvailable);
            DISABLE_STATE(AnalysisState::solutionValid);

            if (dumpFEMinput)
            {
                out << "puts \"Running Pushover...\" ;"  << endl;
                out << "# create push-over pattern:" << endl;
                out << "pattern Plain 200 Linear {"    << endl;
                out << "              sp " << numLoadedNode << " 1 " << HDisp << " ;" << endl;
                out << "              sp " << numLoadedNode << " 3 " << VDisp << " ;" << endl;
                out << "          } ;" << endl;  out << endl;
            }
        };
        break;

    case LoadControlType::SoilMotion:

        // soilNodes is a list of nodeID, depth pairs for soil nodes that are attached to p-y and t-z springs

        if (soilNodes.length() > 0)
        {
            if (dumpFEMinput)
            {
                out << "puts \"Running Soil Motion ...\" ;"  << endl;
                out << "# create soil motion pattern:" << endl;
                out << "pattern Plain 200 Linear {"    << endl;
            }

            foreach (SoilNodeData nd, soilNodes)
            {
                SP_Constraint *theSP = new SP_Constraint(nd.ID, 0, shift(nd.depth), false);
                theLoadPattern->addSP_Constraint(theSP);

                if (dumpFEMinput)
                {
                    // sp $nodeTag $dofTag $dofValue
                    out << "              sp " << nd.ID << " 1 " << shift(nd.depth) << " ;" << endl;
                }
            }

            if (dumpFEMinput) { out << "          } ;" << endl;  out << endl; }

            theDomain->addLoadPattern(theLoadPattern);

            ENABLE_STATE(AnalysisState::loadValid);
            DISABLE_STATE(AnalysisState::solutionAvailable);
            DISABLE_STATE(AnalysisState::solutionValid);
        };
        break;
    }
}

void PileFEAmodeler::buildAnalysis()
{
    if (CHECK_STATE(AnalysisState::analysisValid)) return;

    QTextStream out(FEMfile);

    //
    // create the analysis
    //

    AnalysisModel     *theModel      = new AnalysisModel();
    CTestNormDispIncr *theTest       = new CTestNormDispIncr(1.0e-3, 25, 0);
    EquiSolnAlgo      *theSolnAlgo   = new NewtonRaphson();
    StaticIntegrator  *theIntegrator = new LoadControl(0.05, 1, 0.05, 0.05);
    ConstraintHandler *theHandler    = new PenaltyConstraintHandler(1.0e14, 1.0e14);
    RCM               *theRCM        = new RCM();
    DOF_Numberer      *theNumberer   = new DOF_Numberer(*theRCM);
    BandGenLinSolver  *theSolver     = new BandGenLinLapackSolver();
    LinearSOE         *theSOE        = new BandGenLinSOE(*theSolver);

    if (theAnalysis != nullptr) delete theAnalysis;

    theAnalysis = new StaticAnalysis(*theDomain,
                                  *theHandler,
                                  *theNumberer,
                                  *theModel,
                                  *theSolnAlgo,
                                  *theSOE,
                                  *theIntegrator);
    theSolnAlgo->setConvergenceTest(theTest);

    ENABLE_STATE(AnalysisState::analysisValid);

    if (dumpFEMinput)
    {
        out << endl;
        out << "#----------------------------------------------------------" << endl;
        out << "#  create the analysis                                     " << endl;
        out << "#----------------------------------------------------------" << endl;
        out                                                                  << endl;
        out << "# analysis commands"                                         << endl;
        out << "    integrator LoadControl  0.05 ;"                          << endl;
        out << "    numberer RCM ;"                                          << endl;
        out << "    system BandGeneral ;"                                    << endl;
        out << "    constraints Penalty   1.0e14  1.0e14 ;"                  << endl;
        out << "    test NormDispIncr 1e-5      20      1 ;"                 << endl;
        out << "    algorithm Newton ;"                                      << endl;
        out << "    analysis Static ;"                                       << endl;
        out                                                                  << endl;
        out << "    set startT [clock seconds] ;"                            << endl;
        out << "    puts \"Starting Load Application...\" ;"                 << endl;
        out << "    analyze          200 ;"                                  << endl;
        out                                                                  << endl;
        out << "    set endT [clock seconds] ;"                              << endl;
        out << "    puts \"Load Application finished...\" ;"                 << endl;
        out << "    puts \"Loading Analysis execution time: [expr $endT-$startT] seconds.\" ;" << endl;
        out                                                                  << endl;
        out << "wipe ;"                                                      << endl;
    }
}

void PileFEAmodeler::clearPlotBuffers()
{
    foreach (QVector<double> *ptr, locList) { if (ptr != nullptr) delete ptr; }
    locList.clear();

    foreach (QVector<double> *ptr, lateralDispList) { if (ptr != nullptr) delete ptr; }
    lateralDispList.clear();

    foreach (QVector<double> *ptr, axialDispList) { if (ptr != nullptr) delete ptr; }
    axialDispList.clear();

    foreach (QVector<double> *ptr, MomentList) { if (ptr != nullptr) delete ptr; }
    MomentList.clear();

    foreach (QVector<double> *ptr, ShearList) { if (ptr != nullptr) delete ptr; }
    ShearList.clear();

    foreach (QVector<double> *ptr, AxialList) { if (ptr != nullptr) delete ptr; }
    AxialList.clear();

    foreach (QVector<double> *ptr, StressList) { if (ptr != nullptr) delete ptr; }
    StressList.clear();
}

int PileFEAmodeler::extractPlotData()
{
    if ( CHECK_STATE(AnalysisState::solutionAvailable) && CHECK_STATE(AnalysisState::dataExtracted)) return 0;
    if (!CHECK_STATE(AnalysisState::solutionAvailable)) this->doAnalysis();

    this->clearPlotBuffers();

    for (int pileIdx=0; pileIdx<numPiles; pileIdx++) {

        //
        // allocate storage for results from pile with pileIdx
        //
        locList.append        (new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));
        lateralDispList.append(new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));
        axialDispList.append  (new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));
        MomentList.append     (new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));
        ShearList.append      (new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));
        AxialList.append      (new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));
        StressList.append     (new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));
    }

    if (!CHECK_STATE(AnalysisState::solutionValid)) { return -1; }

    for (int pileIdx=0; pileIdx<numPiles; pileIdx++)
    {
        //
        // collect nodal results
        //
        for (int i=0; i<pileInfo[pileIdx].numNodePile; i++) {
            Node *theNode = theDomain->getNode(i+pileInfo[pileIdx].nodeIDoffset+1);
            const Vector &nodeCoord = theNode->getCrds();
            (*locList[pileIdx])[i] = nodeCoord(2);

            int iLayer;
            for (iLayer=0; iLayer<pileInfo[pileIdx].maxLayers; iLayer++) { if (-nodeCoord(2) <= depthOfLayer[iLayer+1]) break;}
            (*StressList[pileIdx])[i] = mSoilLayers[iLayer].getEffectiveStress(-nodeCoord(2)-depthOfLayer[iLayer]);

            const Vector &nodeDisp = theNode->getDisp();

            (*lateralDispList[pileIdx])[i] = nodeDisp(0);
            (*axialDispList[pileIdx])[i]   = nodeDisp(2);
        }

        /* during initialization, the list can contain zero elements and the next 2 lines will crash */

        /*
         *  identify force components
         *  components identified by *** will be shown in plots
         *
         * eleForces[0] ... Px on node 1 == in plane shear force
         * eleForces[1] ... Py on node 1 == out of plane shear force
         * eleForces[2] ... Pz on node 1 == axial force
         * eleForces[3] ... Mx on node 1 == out of plane bending moment
         * eleForces[4] ... My on node 1 == in plane bending moment
         * eleForces[5] ... Mz on node 1 == torsion
         * eleForces[6] ... Px on node 2 == in plane shear force        ***
         * eleForces[7] ... Py on node 2 == out of plane shear force
         * eleForces[8] ... Pz on node 2 == axial force                 ***
         * eleForces[9] ... Mx on node 2 == out of plane bending moment
         * eleForces[10] .. My on node 2 == in plane bending moment     ***
         * eleForces[11] .. Mz on node 2 == torsion
         */

        //(*MomentList[pileIdx])[0] = 0.0;
        //(*ShearList[pileIdx])[0]  = 0.0;

        for (int i=1; i<pileInfo[pileIdx].numNodePile; i++) {

            Element *theEle = theDomain->getElement(i+pileInfo[pileIdx].elemIDoffset);
            const Vector &eleForces = theEle->getResistingForce();

            (*MomentList[pileIdx])[i] = eleForces(10);
            (*ShearList[pileIdx])[i]  = eleForces(6);
            (*AxialList[pileIdx])[i]  = eleForces(8);
        }
    }

    ENABLE_STATE(AnalysisState::dataExtracted);

    return 0;
}

void PileFEAmodeler::dumpDomain(QString filename)
{
    if (!theDomain) return;

    // ensure we do have a valid FE model
    this->doAnalysis();

    QFile file(filename);

    // Trying to open in WriteOnly and Text mode
    if(!file.open(QFile::WriteOnly |
                  QFile::Text))
    {
        qDebug() << " Could not open file for writing";
        return;
    }

    QTextStream out(&file);

    out << endl << "# nodal coordinates:" << endl;

    NodeIter &nodeItr = theDomain->getNodes();
    Node *node;
    while ((node = nodeItr()) != 0)
    {
        Vector crd(node->getCrds());
        out << "node " << node->getTag() << " " << crd << endl;
    }

    out << endl << "# elements:" << endl;

    ElementIter &elemItr = theDomain->getElements();
    Element *elem;
    while ((elem = elemItr()) != 0)
    {
        out << "element " << elem->getClassType() << " "
            << elem->getTag() << " " ;
        int nNds = elem->getNumExternalNodes();

        ID extNodes = elem->getExternalNodes();

        for (int j=0; j<extNodes.Size(); j++)
        {
            out << extNodes[j] << " ";
        }

        out << " ;" << endl;
    }

    out << endl << "# boundary conditions:" << endl;

    SP_ConstraintIter &SPCitr = theDomain->getSPs();
    SP_Constraint *SPC;
    int nodeTag = -1;
    Vector fixity(6);

    while ((SPC = SPCitr()) != 0)
    {
        if (nodeTag != SPC->getNodeTag())
        {
            out << "fix " << " " << nodeTag << " " << fixity << " ;" << endl;
            nodeTag = SPC->getNodeTag();
            fixity.Zero();
        }
        fixity(SPC->getDOF_Number()) = 1;
    }

    out << endl << "# nodal constraints:" << endl;

    MP_ConstraintIter &MPCitr = theDomain->getMPs();
    MP_Constraint *MPC;
    while ((MPC = MPCitr()) != 0)
    {
        out << "equalDOF " << " " << MPC->getNodeRetained() << " " << MPC->getNodeConstrained() << " ";
        ID cDOFs = MPC->getConstrainedDOFs();
        for (int j=0; j<cDOFs.Size(); j++) { out << " " << cDOFs[j]+1; }
        out << " ;" << endl;
    }

    LoadPatternIter &patternIter = theDomain->getLoadPatterns();

    file.flush();
    file.close();
}

int  PileFEAmodeler::getExitStatus()
{
    return 0;
}

/* ******** return data for plotting or post analysis output ******** *
 *
 * collect data according to the requested type
 *
 * both x and y are QVectors with one QVector<double>entry per pile
 * The length of each associated vactor must match, i.e.,
 *       x[i].size() == y[i].size()
 * BUT
 *       x[i].size() need not match x[j].size()  for i != j
 *
 * ******************************************************************* */

QList<QVector<QVector<double> *> *> PileFEAmodeler::getLateralDisplacements()
{
    QList<QVector<QVector<double> *> *> list;

    if (this->extractPlotData() >= 0)
    {
        list.append(&locList);
        list.append(&lateralDispList);
    }
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getAxialDisplacements()
{
    QList<QVector<QVector<double> *> *> list;

    if (this->extractPlotData() >= 0)
    {
        list.append(&locList);
        list.append(&axialDispList);
    }
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getMoment()
{
    QList<QVector<QVector<double> *> *> list;

    if (this->extractPlotData() >= 0)
    {
        list.append(&locList);
        list.append(&MomentList);
    }
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getShear()
{
    QList<QVector<QVector<double> *> *> list;

    if (this->extractPlotData() >= 0)
    {
        list.append(&locList);
        list.append(&ShearList);
    }
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getForce()
{
    QList<QVector<QVector<double> *> *> list;

    if (this->extractPlotData() >= 0)
    {
        list.append(&locList);
        list.append(&AxialList);
    }
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getStress()
{
    QList<QVector<QVector<double> *> *> list;

    if (this->extractPlotData() >= 0)
    {
        list.append(&locList);
        list.append(&StressList);
    }
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getPult()
{
    QList<QVector<QVector<double> *> *> list;

    if (this->extractPlotData() >= 0)
    {
        list.append(&locList);
        list.append(&pultList);
    }
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getY50()
{
    QList<QVector<QVector<double> *> *> list;

    if (this->extractPlotData() >= 0)
    {
        list.append(&locList);
        list.append(&y50List);
    }
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getTult()
{
    QList<QVector<QVector<double> *> *> list;

    if (this->extractPlotData() >= 0)
    {
        list.append(&locList);
        list.append(&tultList);
    }
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getZ50()
{
    QList<QVector<QVector<double> *> *> list;

    if (this->extractPlotData() >= 0)
    {
        list.append(&locList);
        list.append(&z50List);
    }
    return list;
}

double PileFEAmodeler::shift(double z)
{
    double s = soilMotion.last();

    QVectorIterator<SoilMotionData> itr(motionData);

    while (itr.hasNext())
    {
        SoilMotionData info = itr.next();

        if (info.zmax >= z)
        {
            s = info.delta0 + z * info.delta1;
            break;
        }
    }

    return s;
}

void PileFEAmodeler::updateMotionData(void)
{
    SoilMotionData info;

    for (int i=0; i<MAXLAYERS; i++)
    {
        double si = soilMotion[i];
        double sj = soilMotion[i+1];
        double zi = depthOfLayer[i];
        double zj = depthOfLayer[i+1];

        info.delta1 = (sj - si)/(zj - zi);
        info.delta0 = si - zi * info.delta1;
        info.zmax   = zj;

        motionData[i] = info;
    }
}

void PileFEAmodeler::writeFEMinput(QString filename)
{
    FEMfile = new QFile(filename);

    // Trying to open in WriteOnly and Text mode
    if(!FEMfile->open(QFile::WriteOnly |
                  QFile::Text))
    {
        qDebug() << " Could not open file for writing";
        return;
    }

    dumpFEMinput = true;
    DISABLE_STATE(AnalysisState::meshValid);
    DISABLE_STATE(AnalysisState::loadValid);
    DISABLE_STATE(AnalysisState::analysisValid);

    this->buildMesh();
    this->buildLoad();
    this->buildAnalysis();

    dumpFEMinput = false;

    FEMfile->flush();
    FEMfile->close();
    delete FEMfile;
    FEMfile = nullptr;
}
