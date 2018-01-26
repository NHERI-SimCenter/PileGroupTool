#include "pilefeamodeler.h"

#include <QList>
#include <QListIterator>
#include <QVector>

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


PileFEAmodeler::PileFEAmodeler()
{
    // set default parameters
    modelState.clear();
    modelState.insert("solutionAvailable", false);
    modelState.insert("solutionValid", false);
    modelState.insert("dataExtracted", false);
    modelState.insert("meshValid", false);
    modelState.insert("loadValid", false);
    modelState.insert("analysisValid", false);

    theDomain = new Domain();

    numLoadedNode = -1;

    /*
    locList.clear();
    lateralDispList.clear();
    axialDispList.clear();
    MomentList.clear();
    ShearList.clear();
    AxialList.clear();
    StressList.clear();
    pultList.clear();
    y50List.clear();
    tultList.clear();
    z50List.clear();
    */

    /* set default parameters */
    this->setDefaultParameters();
}

PileFEAmodeler::~PileFEAmodeler()
{

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

    // setup switch
    inSetupState = true;

    // viewer settings
    showDisplacements = true;
    showPullOut       = true;
    showMoments       = true;
    showShear         = true;
    showAxial         = true;
    showStress        = true;
    showPultimate     = true;
    showY50           = true;

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
    DISABLE_STATE("meshValid");
    DISABLE_STATE("loadValid");
    DISABLE_STATE("analysisValid");
    DISABLE_STATE("dataExtracted");
    DISABLE_STATE("solutionValid");
    DISABLE_STATE("solutionAvailable");
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

    DISABLE_STATE("meshValid");
}

void PileFEAmodeler::setLoadType(LoadControlType type)
{
    if (loadControlType != type)
    {
        loadControlType = type;
        DISABLE_STATE("loadValid");
    }
}

void PileFEAmodeler::updateLoad(double Px, double Py, double Moment)
{
    loadControlType = LoadControlType::ForceControl;

    P    = Px;     // lateral force on pile cap
    PV   = Py;     // vertical force on pile cap
    PMom = Moment; // applied moment on pile cap

    DISABLE_STATE("loadValid");
}

void PileFEAmodeler::updateSoil(QVector<soilLayer> &layers)
{
    mSoilLayers = layers;

    DISABLE_STATE("meshValid");
}

void PileFEAmodeler::updateGWtable(double depth)
{
    if (gwtDepth == depth) return;

    gwtDepth = depth;
    DISABLE_STATE("meshValid");
}

void PileFEAmodeler::updateDisplacement(double ux, double uy)
{
    loadControlType = LoadControlType::PushOver;

    HDisp = ux; // prescribed horizontal displacement
    VDisp = uy; // prescriber vertical displacement

    DISABLE_STATE("loadValid");
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

    DISABLE_STATE("loadValid");
}

void PileFEAmodeler::setAnalysisType(QString)
{

    DISABLE_STATE("analysisValid");
}

void PileFEAmodeler::doAnalysis()
{
    if (!CHECK_STATE("meshValid")) this->buildMesh();
    if (!CHECK_STATE("loadValid")) this->buildLoad();
    if (!CHECK_STATE("analysisValid")) this->buildAnalysis();

    DISABLE_STATE("dataExtracted");
    DISABLE_STATE("solutionValid");
    DISABLE_STATE("solutionAvailable");

    //
    //analyze & get results
    //
    theAnalysis->analyze(20);
    theDomain->calculateNodalReactions(0);

    ENABLE_STATE("solutionAvailable");
}

void PileFEAmodeler::buildMesh()
{
    //QVector<HEAD_NODE_TYPE> headNodeList(MAXPILES, {-1,-1,0.0, 1.0, 1.0});

    for (int k=0; k<MAXPILES; k++) {
        headNodeList[k] = {-1, -1, 0.0, 1.0, 1.0};
    }

    if (inSetupState) return;

    // clear existing model
    theDomain->clearAll();
    OPS_clearAllUniaxialMaterial();
    ops_Dt = 0.0;

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

    //this->updateSystemPlot();

    QVector<QVector<double> > locList(MAXPILES, QVector<double>(numNodePiles,0.0));
    QVector<QVector<double> > pultList(MAXPILES, QVector<double>(numNodePiles,0.0));
    QVector<QVector<double> > y50List(MAXPILES, QVector<double>(numNodePiles,0.0));

    int ioffset  = numNodePiles;              // for p-y spring nodes
    int ioffset2 = ioffset + numNodePiles;    // for pile nodes
    int ioffset3 = ioffset2 + numNodePiles;   // for t-z spring nodes
    int ioffset4 = ioffset3 + numNodePiles;   // for toe resistance nodes
    int ioffset5 = ioffset4 + numNodePiles;   // for pile cap nodes

    /* ******** build the finite element mesh ******** */

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
        //qDebug() << "+ pile index: " << pileIdx;

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

        //qDebug() << "Node(" << nodeTag << "," << 6 << "," << xOffset[pileIdx] << "," << 0.0 << "," << zCoord << ")";

        theNode = new Node(nodeTag, 6, pileInfo[pileIdx].xOffset, 0., zCoord);  theDomain->addNode(theNode);
        if (numNode != 1) {
            SP_Constraint *theSP = 0;
            theSP = new SP_Constraint(nodeTag, 1, 0., true); theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(nodeTag, 3, 0., true); theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(nodeTag, 5, 0., true); theDomain->addSP_Constraint(theSP);
        }

        //
        // add toe resistance (if requested)
        //
    /*
     *  CHECK IN WITH FRANK:
     *     how to properly implement a Q-z spring at the end WITHOUT the p-y spring.
     */

        if (useToeResistance) {
            Node *theNode = 0;

            theNode = new Node(numNode,         3, pileInfo[pileIdx].xOffset, 0., zCoord);  theDomain->addNode(theNode);
            theNode = new Node(numNode+ioffset, 3, pileInfo[pileIdx].xOffset, 0., zCoord);  theDomain->addNode(theNode);

            SP_Constraint *theSP = 0;
            theSP = new SP_Constraint(numNode, 0, 0., true);  theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode, 1, 0., true);  theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode, 2, 0., true);  theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode+ioffset, 0, 0., true);  theDomain->addSP_Constraint(theSP); // ?
            theSP = new SP_Constraint(numNode+ioffset, 1, 0., true);  theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode+ioffset, 2, 0., true);  theDomain->addSP_Constraint(theSP);
        }

        locList[pileIdx][numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset]  = zCoord;
        pultList[pileIdx][numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset] = 0.001;
        y50List[pileIdx][numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset]  = 0.00001;

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

                SP_Constraint *theSP = 0;
                theSP = new SP_Constraint(numNode, 0, 0., true);  theDomain->addSP_Constraint(theSP);
                theSP = new SP_Constraint(numNode, 1, 0., true);  theDomain->addSP_Constraint(theSP);
                theSP = new SP_Constraint(numNode, 2, 0., true);  theDomain->addSP_Constraint(theSP);
                theSP = new SP_Constraint(numNode+ioffset, 1, 0., true);  theDomain->addSP_Constraint(theSP);
                theSP = new SP_Constraint(numNode+ioffset, 2, 0., true);  theDomain->addSP_Constraint(theSP);

                //
                // pile nodes
                //

                nodeTag = numNode+ioffset2;

                //qDebug() << "Node(" << nodeTag << "," << 6 << "," << xOffset[pileIdx] << "," << 0.0 << "," << zCoord << ")";

                theNode = new Node(nodeTag, 6, pileInfo[pileIdx].xOffset, 0., zCoord);  theDomain->addNode(theNode);
                if (numNode != 1) {
                    SP_Constraint *theSP = 0;
                    theSP = new SP_Constraint(nodeTag, 1, 0., true); theDomain->addSP_Constraint(theSP);
                    theSP = new SP_Constraint(nodeTag, 3, 0., true); theDomain->addSP_Constraint(theSP);
                    theSP = new SP_Constraint(nodeTag, 5, 0., true); theDomain->addSP_Constraint(theSP);
                }

                // constrain spring and pile nodes with equalDOF (identity constraints)
                MP_Constraint *theMP = new MP_Constraint(numNode+ioffset2, numNode+ioffset, Ccr, rcDof, rcDof);
                theDomain->addMP_Constraint(theMP);

                //
                // create soil-spring materials
                //

                // # p-y spring material
                puSwitch  = 2;  // Hanson
                //puSwitch  = 1;  // API // temporary switch
                kSwitch   = 1;  // API

                gwtSwitch = (gwtDepth > -zCoord)?1:2;

                int dummy = iLayer;

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

                locList[pileIdx][numNode+ioffset2- pileInfo[pileIdx].nodeIDoffset]  = zCoord;
                // pult is a nodal value for the p-y spring.
                // It needs to be scaled by element length ito represent a line load
                pultList[pileIdx][numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset] = pult/eleSize;
                y50List[pileIdx][numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset]  = y50;

                // t-z spring material
                getTzParam(phi, pileInfo[pileIdx].pileDiameter,  sigV,  eleSize, &tult, &z50);

                if(tult <= 0.0 || z50 <= 0.0) {
                    qDebug() << "WARNING -- only accepts positive nonzero tult and z50";
                    qDebug() << "*** iLayer: " << iLayer << "   pile number" << pileIdx+1
                             << "   depth: " << -zCoord << "   sigV: "  << sigV
                             << "   diameter: " << pileInfo[pileIdx].pileDiameter << "   eleSize: " << eleSize;
                    qDebug() << "*** tult: " << tult << "   z50: " << z50;
                }

                theMat = new TzSimple1(numNode+ioffset, 0, 2, tult, z50, 0.0);
                OPS_addUniaxialMaterial(theMat);

                //
                // create soil spring elements
                //

                UniaxialMaterial *theMaterials[2];
                theMaterials[0] = OPS_getUniaxialMaterial(numNode);
                theMaterials[1] = OPS_getUniaxialMaterial(numNode+ioffset);
                Element *theEle = new ZeroLength(numNode+ioffset3, 3, numNode, numNode+ioffset, x, y, 2, theMaterials, direction);
                theDomain->addElement(theEle);

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

            //qDebug() << "Node(" << nodeTag << "," << 6 << "," << xOffset[pileIdx] << "," << 0.0 << "," << zCoord << ")";

            Node *theNode = new Node(nodeTag, 6, pileInfo[pileIdx].xOffset, 0., zCoord);  theDomain->addNode(theNode);
            if (numNode != 1) {
                SP_Constraint *theSP = 0;
                theSP = new SP_Constraint(nodeTag, 1, 0., true); theDomain->addSP_Constraint(theSP);
                theSP = new SP_Constraint(nodeTag, 3, 0., true); theDomain->addSP_Constraint(theSP);
                theSP = new SP_Constraint(nodeTag, 5, 0., true); theDomain->addSP_Constraint(theSP);
            }

            locList[pileIdx][numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset]  = zCoord;
            pultList[pileIdx][numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset] = 0.001;
            y50List[pileIdx][numNode+ioffset2-pileInfo[pileIdx].nodeIDoffset]  = 0.00001;

            zCoord += eleSize;
        }


        headNodeList[pileIdx] = {pileIdx, nodeTag, pileInfo[pileIdx].xOffset};

        //
        // create pile elements
        //

        static Vector crdV(3); crdV(0)=0.; crdV(1)=-1; crdV(2) = 0.;
        CrdTransf *theTransformation = new LinearCrdTransf3d(1, crdV);

        for (int i=0; i<pileInfo[pileIdx].numNodePile-1; i++) {
            numElem++;
            BeamIntegration *theIntegration = new LegendreBeamIntegration();
            SectionForceDeformation *theSections[3];
            SectionForceDeformation *theSection = new ElasticSection3d(pileIdx+1, pileInfo[pileIdx].E, A, Iz, Iz, G, J);
            theSections[0] = theSection;
            theSections[1] = theSection;
            theSections[2] = theSection;

            //qDebug() << "DispBeamColumn3d("
            //         << numElem << ","
            //         << nodeIDoffset[pileIdx]+i+1 << ","
            //         << nodeIDoffset[pileIdx]+i+2 << ")";

            Element *theEle = new DispBeamColumn3d(numElem,
                                                   pileInfo[pileIdx].nodeIDoffset+i+1,
                                                   pileInfo[pileIdx].nodeIDoffset+i+2,
                                                   3, theSections, *theIntegration, *theTransformation);
            theDomain->addElement(theEle);
            //delete theSection;
            //delete theIntegration;
        }

        //
        // set up toe resistance, if requested
        //
        if (useToeResistance) {

            // # q-z spring material
            // # vertical effective stress at pile tip, no water table (depth is embedded pile length)
            double sigVq  = mSoilLayers[pileInfo[pileIdx].maxLayers-1].getLayerBottomStress();
            double phi  = mSoilLayers[pileInfo[pileIdx].maxLayers-1].getLayerFrictionAng();

            getQzParam(phi, pileInfo[pileIdx].pileDiameter,  sigVq,  gSoil, &qult, &z50q);
            UniaxialMaterial *theMat = new QzSimple1(1+ioffset, 2, qult, z50q, 0.0, 0.0);
            OPS_addUniaxialMaterial(theMat);

            ID Onedirection(1);
            Onedirection[0] = 2;

            // pile toe
            Element *theEle = new ZeroLength(1+pileIdx+ioffset4, 3, 1, 1+ioffset, x, y, 1, &theMat, Onedirection);
            theDomain->addElement(theEle);
        }
    }

    //
    // *** construct the pile head ***
    //
    if (numPiles > 0) {

        // sort piles by xOffset

        for (int i=0; i<numPiles; i++) {
            for (int j=0; j<numPiles-i; j++) {
                if (headNodeList[i].x < headNodeList[j].x) SWAP(headNodeList[i],headNodeList[j]);
            }
        }

        // set up transformation and orientation for the pile cap elements

        static Vector crdV(3); crdV(0)=0.; crdV(1)=-1; crdV(2) = 0.;
        CrdTransf *theTransformation = new LinearCrdTransf3d(1, crdV);

        // define beam element integration and cross sections

        BeamIntegration *theIntegration = new LegendreBeamIntegration();
        SectionForceDeformation *theSections[3];
        SectionForceDeformation *theSection = new ElasticSection3d(numPiles+1, 1., EA, EI, EI, 1., GJ);
        theSections[0] = theSection;
        theSections[1] = theSection;
        theSections[2] = theSection;

        int prevNode = -1;

        for (int pileIdx=0; pileIdx<numPiles; pileIdx++) {

            // create node for pile head
            numNode++;
            int nodeTag = numNode + ioffset5;

            //qDebug() << "Node(" << nodeTag << "," << 6 << "," << headNodeList[pileIdx].x << "," << 0.0 << "," << L1 << ")";

            Node *theNode = new Node(nodeTag, 6, headNodeList[pileIdx].x, 0., pileInfo[pileIdx].L1);  theDomain->addNode(theNode);

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
            }

            // create beams for pile head
            if (prevNode > 0) {
                numElem++;

                //qDebug() << "DispBeamColumn3d(" << numElem << "," << prevNode << "," << numNode << ")";

                Element *theEle = new DispBeamColumn3d(numElem, prevNode, nodeTag,
                                                       3, theSections, *theIntegration, *theTransformation);
                theDomain->addElement(theEle);
            }

            prevNode = nodeTag;
        }
    }

    if (numPiles == 1) {
        /* need extra fixities to prevent singular system matrix */
        int nodeTag = numNode + ioffset5;

        SP_Constraint *theSP = 0;
        theSP = new SP_Constraint(nodeTag, 4, 0., true); theDomain->addSP_Constraint(theSP);
    }

    numLoadedNode = headNodeList[0].nodeIdx;

    /* *** done with the pile head *** */

    if (numNode != numNodePiles) {
        qDebug() << "ERROR: " << numNode << " nodes generated but " << numNodePiles << "expected" << endln;
    }

    ENABLE_STATE("meshValid");

}

void PileFEAmodeler::buildLoad()
{
    //
    // create load pattern and add loads
    //

    LinearSeries *theTimeSeries = new LinearSeries(1, 1.0);
    LoadPattern *theLoadPattern = new LoadPattern(1);
    theLoadPattern->setTimeSeries(theTimeSeries);
    static Vector load(6); load.Zero(); load(0) = P;
    NodalLoad *theLoad = new NodalLoad(0, numLoadedNode, load);
    theLoadPattern->addNodalLoad(theLoad);
    theDomain->addLoadPattern(theLoadPattern);

    ENABLE_STATE("loadValid");
}

void PileFEAmodeler::buildAnalysis()
{
    if (CHECK_STATE("analysisValid")) return;

    //
    // create the analysis
    //

    AnalysisModel     *theModel      = new AnalysisModel();
    CTestNormDispIncr *theTest       = new CTestNormDispIncr(1.0e-3, 20, 0);
    EquiSolnAlgo      *theSolnAlgo   = new NewtonRaphson();
    StaticIntegrator  *theIntegrator = new LoadControl(0.05, 1, 0.05, 0.05);
    //ConstraintHandler *theHandler    = new TransformationConstraintHandler();
    ConstraintHandler *theHandler    = new PenaltyConstraintHandler(1.0e14, 1.0e14);
    RCM               *theRCM        = new RCM();
    DOF_Numberer      *theNumberer   = new DOF_Numberer(*theRCM);
    BandGenLinSolver  *theSolver     = new BandGenLinLapackSolver();
    LinearSOE         *theSOE        = new BandGenLinSOE(*theSolver);

    if (theAnalysis != NULL) delete theAnalysis;

    theAnalysis = new StaticAnalysis(*theDomain,
                                  *theHandler,
                                  *theNumberer,
                                  *theModel,
                                  *theSolnAlgo,
                                  *theSOE,
                                  *theIntegrator);
    theSolnAlgo->setConvergenceTest(theTest);

    ENABLE_STATE("analysisValid");
}


void PileFEAmodeler::clearPlotBuffers()
{
    foreach (QVector<double> *ptr, locList) { if (ptr != NULL) delete ptr; }
    locList.clear();

    foreach (QVector<double> *ptr, lateralDispList) { if (ptr != NULL) delete ptr; }
    lateralDispList.clear();

    foreach (QVector<double> *ptr, axialDispList) { if (ptr != NULL) delete ptr; }
    axialDispList.clear();

    foreach (QVector<double> *ptr, MomentList) { if (ptr != NULL) delete ptr; }
    MomentList.clear();

    foreach (QVector<double> *ptr, ShearList) { if (ptr != NULL) delete ptr; }
    ShearList.clear();

    foreach (QVector<double> *ptr, AxialList) { if (ptr != NULL) delete ptr; }
    AxialList.clear();

    foreach (QVector<double> *ptr, pultList) { if (ptr != NULL) delete ptr; }
    pultList.clear();

    foreach (QVector<double> *ptr, y50List) { if (ptr != NULL) delete ptr; }
    y50List.clear();

    foreach (QVector<double> *ptr, tultList) { if (ptr != NULL) delete ptr; }
    tultList.clear();

    foreach (QVector<double> *ptr, z50List) { if (ptr != NULL) delete ptr; }
    z50List.clear();
}

void PileFEAmodeler::extractPlotData()
{
    if ( CHECK_STATE("dataExtracted")) return;

    if (!CHECK_STATE("solutionAvailable")) this->doAnalysis();

    this->clearPlotBuffers();

    /*
    double maxHDisp  = 0.0;
    double minHDisp  = 0.0;
    double maxVDisp  = 0.0;
    double minVDisp  = 0.0;
    double maxShear  = 0.0;
    double minShear  = 0.0;
    double maxAxial  = 0.0;
    double minAxial  = 0.0;
    double maxMoment = 0.0;
    double minMoment = 0.0;
    */

    int pileIdx;

    for (pileIdx=0; pileIdx<numPiles; pileIdx++) {

        //
        // allocate storage for results from pile with pileIdx
        //
        locList.append(new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));
        lateralDispList.append(new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));
        axialDispList.append(new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));
        MomentList.append(new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));
        ShearList.append(new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));
        AxialList.append(new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));

        StressList.append(new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));

        pultList.append(new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));
        y50List.append(new QVector<double>(pileInfo[pileIdx].numNodePile,  0.0));
        tultList.append(new QVector<double>(pileInfo[pileIdx].numNodePile, 0.0));
        z50List.append(new QVector<double>(pileInfo[pileIdx].numNodePile,  0.0));

        //
        // collect nodal results
        //
        for (int i=0; i<pileInfo[pileIdx].numNodePile; i++) {
            Node *theNode = theDomain->getNode(i+pileInfo[pileIdx].nodeIDoffset);
            const Vector &nodeCoord = theNode->getCrds();
            (*locList[pileIdx])[i] = nodeCoord(2);
            int iLayer;
            for (iLayer=0; iLayer<pileInfo[pileIdx].maxLayers; iLayer++) { if (-nodeCoord(2) <= depthOfLayer[iLayer+1]) break;}
            (*StressList[pileIdx])[i] = mSoilLayers[iLayer].getEffectiveStress(-nodeCoord(2)-depthOfLayer[iLayer]);
            //if (stress[pileIdx][i] > maxStress) maxStress = stress[pileIdx][i];
            //if (stress[pileIdx][i] < minStress) minStress = stress[pileIdx][i];

            const Vector &nodeDisp = theNode->getDisp();

            (*lateralDispList[pileIdx])[i] = nodeDisp(0);
            //if ((*lateralDispList[pileIdx])[i] > maxHDisp) maxHDisp = (*lateralDispList[pileIdx])[i];
            //if ((*lateralDispList[pileIdx])[i] < minHDisp) minHDisp = (*lateralDispList[pileIdx])[i];

            (*axialDispList[pileIdx])[i] = nodeDisp(2);
            //if ((*axialDispList[pileIdx])[i] > maxVDisp) maxVDisp = (*axialDispList[pileIdx])[i];
            //if ((*axialDispList[pileIdx])[i] < minVDisp) minVDisp = (*axialDispList[pileIdx])[i];
        }

        (*MomentList[pileIdx])[0] = 0.0;
        (*ShearList[pileIdx])[0]  = 0.0;

        for (int i=1; i<pileInfo[pileIdx].numNodePile; i++) {

            Element *theEle = theDomain->getElement(i+pileInfo[pileIdx].elemIDoffset);
            const Vector &eleForces = theEle->getResistingForce();

            (*MomentList[pileIdx])[i] = eleForces(10);
            //if ((*MomentList[pileIdx])[i] > maxMoment) maxMoment = (*MomentList[pileIdx])[i];
            //if ((*MomentList[pileIdx])[i] < minMoment) minMoment = (*MomentList[pileIdx])[i];

            (*ShearList[pileIdx])[i] = eleForces(6);
            //if ((*ShearList[pileIdx])[i] > maxShear) maxShear = (*ShearList[pileIdx])[i];
            //if ((*ShearList[pileIdx])[i] < minShear) minShear = (*ShearList[pileIdx])[i];
        }
    }

    ENABLE_STATE("dataExtracted");
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
    this->extractPlotData();
    QList<QVector<QVector<double> *> *> list;
    list.append(&locList);
    list.append(&lateralDispList);
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getAxialDisplacements()
{
    this->extractPlotData();
    QList<QVector<QVector<double> *> *> list;
    list.append(&locList);
    list.append(&axialDispList);
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getMoment()
{
    this->extractPlotData();
    QList<QVector<QVector<double> *> *> list;
    list.append(&locList);
    list.append(&MomentList);
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getShear()
{
    this->extractPlotData();
    QList<QVector<QVector<double> *> *> list;
    list.append(&locList);
    list.append(&ShearList);
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getForce()
{
    this->extractPlotData();
    QList<QVector<QVector<double> *> *> list;
    list.append(&locList);
    list.append(&AxialList);
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getStress()
{
    this->extractPlotData();
    QList<QVector<QVector<double> *> *> list;
    list.append(&locList);
    list.append(&StressList);
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getPult()
{
    this->extractPlotData();
    QList<QVector<QVector<double> *> *> list;
    list.append(&locList);
    list.append(&pultList);
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getY50()
{
    this->extractPlotData();
    QList<QVector<QVector<double> *> *> list;
    list.append(&locList);
    list.append(&y50List);
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getTult()
{
    this->extractPlotData();
    QList<QVector<QVector<double> *> *> list;
    list.append(&locList);
    list.append(&tultList);
    return list;
}

QList<QVector<QVector<double> *> *> PileFEAmodeler::getZ50()
{
    this->extractPlotData();
    QList<QVector<QVector<double> *> *> list;
    list.append(&locList);
    list.append(&z50List);
    return list;
}

