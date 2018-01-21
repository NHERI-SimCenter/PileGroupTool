#include "pilefeamodeler.h"

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


#define CHECK_STATE(X)   modelState.value(X)
#define ENABLE_STATE(X)  modelState[X]=true
#define DISABLE_STATE(X) modelState[X]=false


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

    /* set default parameters */
    this->setDefaultParameters();
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

    L1 = 0.00;               // pile length above ground (all the same)
    numNodePiles = 0
            ;
    for (int k=0; k<MAXPILES;k++)
    {
        numNodePile[k]  = 0;
        maxLayers[k]    = 0;
        nodeIDoffset[k] = 0;
        elemIDoffset[k] = 0;

        L2[k]           = 10.0;      // embedded length of pile
        pileDiameter[k] = 1.00;      // pile diameter
        E[k]            = 1.00;      // pile modulus of elasticity
        xOffset[k]      = double(k); // x-offset of pile
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

void PileFEAmodeler::updatePiles(QMap<QString, double> &pileInfo)
{
    DISABLE_STATE("meshValid");
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
        numNodePile[k]  = 0;
        maxLayers[k]    = MAXLAYERS;
        nodeIDoffset[k] = 0;
        elemIDoffset[k] = 0;
    }

    /* ******** sizing and adjustments ******** */

    for (int pileIdx=0; pileIdx<numPiles; pileIdx++)
    {
        numNodePile[pileIdx] = 2; // bottom plus surface node
        if (L1 > 0.0001) numNodePile[pileIdx] += numElementsInAir; // free standing

        //
        // find active layers for pile #pileIdx
        //
        for (int iLayer=0; iLayer < maxLayers[pileIdx]; iLayer++)
        {
            if (depthOfLayer[iLayer] >= L2[pileIdx]) {
                // this pile only penetrates till layer #iLayer
                maxLayers[pileIdx] = iLayer;
                break;
            }

            double thickness = mSoilLayers[iLayer].getLayerThickness();

            // compute bottom of this layer/top of the next layer
            if (depthOfLayer[iLayer] + thickness < L2[pileIdx] && iLayer == MAXLAYERS - 1) {
                thickness = L2[pileIdx] - depthOfLayer[iLayer];
                mSoilLayers[iLayer].setLayerThickness(thickness);
            }
            depthOfLayer[iLayer+1] = depthOfLayer[iLayer] + thickness;

            // check if layer ends below the pile toe and adjust thickness for mesh generation
            if (depthOfLayer[iLayer+1] > L2[pileIdx]) {
                thickness = L2[pileIdx] - depthOfLayer[iLayer];
            }

            int numElemThisLayer = int(thickness/pileDiameter[pileIdx]);
            if (numElemThisLayer < minElementsPerLayer) numElemThisLayer = minElementsPerLayer;
            if (numElemThisLayer > maxElementsPerLayer) numElemThisLayer = maxElementsPerLayer;

            // remember number of elements in this layer
            elemsInLayer[pileIdx][iLayer] = numElemThisLayer;

            // total node count
            numNodePile[pileIdx] += numElemThisLayer;

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
        numNodePiles += numNodePile[pileIdx];
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

    nodeIDoffset[0] = ioffset2;
    elemIDoffset[0] = ioffset2;
    for (int k=1; k<numPiles; k++) {
        nodeIDoffset[k] = nodeIDoffset[k-1] + numNodePile[k-1];
        elemIDoffset[k] = elemIDoffset[k-1] + numNodePile[k-1] - 1;
    }

    // build the FE-model one pile at a time

    for (int pileIdx=0; pileIdx<numPiles; pileIdx++)
    {
        //qDebug() << "+ pile index: " << pileIdx;

        //
        // compute pile properties (compute once; used for all pile elements)
        //
        double PI = 3.14159;
        double A  = 0.2500 * PI * pileDiameter[pileIdx] * pileDiameter[pileIdx];
        double Iz = 0.0625 *  A * pileDiameter[pileIdx] * pileDiameter[pileIdx];
        double G  = E[pileIdx]/(2.0*(1.+0.3));
        double J  = 1.0e10;

        // suitable pile head parameters (make pile head stiff)
        if (100.*E[pileIdx]*A > EA ) EA = 100.*E[pileIdx]*A;
        if (100.*E[pileIdx]*Iz > EI) EI = 100.*E[pileIdx]*Iz;
        if (10.*G*J > GJ)            GJ = 10.*G*J;

        //
        // Ready to generate the structure
        //

        /* embedded pile portion */

        zCoord = -L2[pileIdx];

        //
        // create bottom pile node
        //
        numNode += 1;

        Node *theNode = 0;
        nodeTag = numNode+ioffset2;

        //qDebug() << "Node(" << nodeTag << "," << 6 << "," << xOffset[pileIdx] << "," << 0.0 << "," << zCoord << ")";

        theNode = new Node(nodeTag, 6, xOffset[pileIdx], 0., zCoord);  theDomain->addNode(theNode);
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

            theNode = new Node(numNode,         3, xOffset[pileIdx], 0., zCoord);  theDomain->addNode(theNode);
            theNode = new Node(numNode+ioffset, 3, xOffset[pileIdx], 0., zCoord);  theDomain->addNode(theNode);

            SP_Constraint *theSP = 0;
            theSP = new SP_Constraint(numNode, 0, 0., true);  theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode, 1, 0., true);  theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode, 2, 0., true);  theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode+ioffset, 0, 0., true);  theDomain->addSP_Constraint(theSP); // ?
            theSP = new SP_Constraint(numNode+ioffset, 1, 0., true);  theDomain->addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode+ioffset, 2, 0., true);  theDomain->addSP_Constraint(theSP);
        }

        locList[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]]  = zCoord;
        pultList[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]] = 0.001;
        y50List[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]]  = 0.00001;

        //
        // work the way up layer by layer
        //

        for (int iLayer=maxLayers[pileIdx]-1; iLayer >= 0; iLayer--)
        {
            double thickness = mSoilLayers[iLayer].getLayerThickness();
            if (depthOfLayer[iLayer+1] > L2[pileIdx]) {
                thickness = L2[pileIdx] - depthOfLayer[iLayer];
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

                theNode = new Node(numNode,         3, xOffset[pileIdx], 0., zCoord);  theDomain->addNode(theNode);
                theNode = new Node(numNode+ioffset, 3, xOffset[pileIdx], 0., zCoord);  theDomain->addNode(theNode);

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

                theNode = new Node(nodeTag, 6, xOffset[pileIdx], 0., zCoord);  theDomain->addNode(theNode);
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
                getPyParam(-zCoord, sigV, phi, pileDiameter[pileIdx], eleSize, puSwitch, kSwitch, gwtSwitch, &pult, &y50);

                if(pult <= 0.0 || y50 <= 0.0) {
                    qDebug() << "WARNING -- only accepts positive nonzero pult and y50";
                    qDebug() << "*** iLayer: " << iLayer << "   pile number" << pileIdx+1
                             << "   depth: " << -zCoord
                             << "   depth in layer: " << depthInLayer
                             << "   sigV: "  << sigV
                             << "   diameter: " << pileDiameter[pileIdx] << "   eleSize: " << eleSize;
                    qDebug() << "*** pult: " << pult << "   y50: " << y50;
                }

                theMat = new PySimple1(numNode, 0, 2, pult, y50, 0.0, 0.0);
                OPS_addUniaxialMaterial(theMat);

                locList[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]]  = zCoord;
                // pult is a nodal value for the p-y spring.
                // It needs to be scaled by element length ito represent a line load
                pultList[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]] = pult/eleSize;
                y50List[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]]  = y50;

                // t-z spring material
                getTzParam(phi, pileDiameter[pileIdx],  sigV,  eleSize, &tult, &z50);

                if(tult <= 0.0 || z50 <= 0.0) {
                    qDebug() << "WARNING -- only accepts positive nonzero tult and z50";
                    qDebug() << "*** iLayer: " << iLayer << "   pile number" << pileIdx+1
                             << "   depth: " << -zCoord << "   sigV: "  << sigV
                             << "   diameter: " << pileDiameter[pileIdx] << "   eleSize: " << eleSize;
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

        if (L1 > 0.0001) {
            eleSize = L1 / (1.0*numElementsInAir);
            zCoord = 0.0;
        }
        else {
            eleSize = 999.99;
            zCoord = 0.0;
        }

        while (zCoord < (L1 + 1.0e-6)) {
            numNode += 1;

            nodeTag = numNode+ioffset2;

            //qDebug() << "Node(" << nodeTag << "," << 6 << "," << xOffset[pileIdx] << "," << 0.0 << "," << zCoord << ")";

            Node *theNode = new Node(nodeTag, 6, xOffset[pileIdx], 0., zCoord);  theDomain->addNode(theNode);
            if (numNode != 1) {
                SP_Constraint *theSP = 0;
                theSP = new SP_Constraint(nodeTag, 1, 0., true); theDomain->addSP_Constraint(theSP);
                theSP = new SP_Constraint(nodeTag, 3, 0., true); theDomain->addSP_Constraint(theSP);
                theSP = new SP_Constraint(nodeTag, 5, 0., true); theDomain->addSP_Constraint(theSP);
            }

            locList[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]]  = zCoord;
            pultList[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]] = 0.001;
            y50List[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]]  = 0.00001;

            zCoord += eleSize;
        }


        headNodeList[pileIdx] = {pileIdx, nodeTag, xOffset[pileIdx]};

        //
        // create pile elements
        //

        static Vector crdV(3); crdV(0)=0.; crdV(1)=-1; crdV(2) = 0.;
        CrdTransf *theTransformation = new LinearCrdTransf3d(1, crdV);

        for (int i=0; i<numNodePile[pileIdx]-1; i++) {
            numElem++;
            BeamIntegration *theIntegration = new LegendreBeamIntegration();
            SectionForceDeformation *theSections[3];
            SectionForceDeformation *theSection = new ElasticSection3d(pileIdx+1, E[pileIdx], A, Iz, Iz, G, J);
            theSections[0] = theSection;
            theSections[1] = theSection;
            theSections[2] = theSection;

            //qDebug() << "DispBeamColumn3d("
            //         << numElem << ","
            //         << nodeIDoffset[pileIdx]+i+1 << ","
            //         << nodeIDoffset[pileIdx]+i+2 << ")";

            Element *theEle = new DispBeamColumn3d(numElem,
                                                   nodeIDoffset[pileIdx]+i+1,
                                                   nodeIDoffset[pileIdx]+i+2,
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
            double sigVq  = mSoilLayers[maxLayers[pileIdx]-1].getLayerBottomStress();
            double phi  = mSoilLayers[maxLayers[pileIdx]-1].getLayerFrictionAng();

            getQzParam(phi, pileDiameter[pileIdx],  sigVq,  gSoil, &qult, &z50q);
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

            Node *theNode = new Node(nodeTag, 6, headNodeList[pileIdx].x, 0., L1);  theDomain->addNode(theNode);

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

void PileFEAmodeler::extractPlotData()
{
    if (!CHECK_STATE("solutionAvailable")) this->doAnalysis();
    if (!CHECK_STATE("dataExtracted")) this->extractPlotData();

#if 1
    QVector<QVector<double>> loc(MAXPILES, QVector<double>(numNodePiles,0.0));
    QVector<QVector<double>> disp(MAXPILES, QVector<double>(numNodePiles,0.0));
    QVector<QVector<double>> moment(MAXPILES, QVector<double>(numNodePiles,0.0));
    QVector<QVector<double>> shear(MAXPILES, QVector<double>(numNodePiles,0.0));
    QVector<QVector<double>> stress(MAXPILES, QVector<double>(numNodePiles,0.0));
    QVector<double> zero(numNodePiles,0.0);
#else
    QVector<QVector<double> *> loc(MAXPILES, 0);
    QVector<QVector<double> *> disp(MAXPILES, 0);
    QVector<QVector<double> *> moment(MAXPILES, 0);
    QVector<QVector<double> *> shear(MAXPILES, 0);
    QVector<QVector<double> *> stress(MAXPILES, 0);
    QVector<double> zero(numNodePiles,0.0);

    for (int k=0; k<numPiles; k++) {
        if (loc[k] != NULL)    delete loc[k];
        if (disp[k] != NULL)   delete disp[k];
        if (moment[k] != NULL) delete moment[k];
        if (shear[k] != NULL)  delete shear[k];
        if (stress[k] != NULL) delete stress[k];
        loc[k]    = new QVector<double>(numNodePile[k],0.0);
        disp[k]   = new QVector<double>(numNodePile[k],0.0);
        moment[k] = new QVector<double>(numNodePile[k],0.0);
        shear[k]  = new QVector<double>(numNodePile[k],0.0);
        stress[k] = new QVector<double>(numNodePile[k],0.0);
    }
#endif

    double maxDisp   = 0.0;
    double minDisp   = 0.0;
    double maxShear  = 0.0;
    double minShear  = 0.0;
    double maxMoment = 0.0;
    double minMoment = 0.0;

    int pileIdx;

    for (pileIdx=0; pileIdx<numPiles; pileIdx++) {

        for (int i=1; i<=numNodePile[pileIdx]; i++) {
            //zero[i-1] = 0.0;

            //qDebug() << "getNode(" << i+nodeIDoffset[pileIdx] << ")";

            Node *theNode = theDomain->getNode(i+nodeIDoffset[pileIdx]);
            const Vector &nodeCoord = theNode->getCrds();
            loc[pileIdx][i-1] = nodeCoord(2);
            int iLayer;
            for (iLayer=0; iLayer<maxLayers[pileIdx]; iLayer++) { if (-nodeCoord(2) <= depthOfLayer[iLayer+1]) break;}
            stress[pileIdx][i-1] = mSoilLayers[iLayer].getEffectiveStress(-nodeCoord(2)-depthOfLayer[iLayer]);
            //if (stress[pileIdx][i-1] > maxStress) maxStress = stress[pileIdx][i-1];
            //if (stress[pileIdx][i-1] < minStress) minStress = stress[pileIdx][i-1];
            const Vector &nodeDisp = theNode->getDisp();
            disp[pileIdx][i-1] = nodeDisp(0);
            if (disp[pileIdx][i-1] > maxDisp) maxDisp = disp[pileIdx][i-1];
            if (disp[pileIdx][i-1] < minDisp) minDisp = disp[pileIdx][i-1];
        }
    }

    for (pileIdx=0; pileIdx<numPiles; pileIdx++) {

        //qDebug() << "= pile index: " << pileIdx ;

        moment[pileIdx][0] = 0.0;
        shear[pileIdx][0]  = 0.0;

        for (int i=1; i<numNodePile[pileIdx]; i++) {

            //qDebug() << "getElement(" << i+elemIDoffset[pileIdx] << ")";

            Element *theEle = theDomain->getElement(i+elemIDoffset[pileIdx]);
            const Vector &eleForces = theEle->getResistingForce();
            moment[pileIdx][i] = eleForces(10);
            if (moment[pileIdx][i] > maxMoment) maxMoment = moment[pileIdx][i];
            if (moment[pileIdx][i] < minMoment) minMoment = moment[pileIdx][i];
            shear[pileIdx][i] = eleForces(6);
            if (shear[pileIdx][i] > maxShear) maxShear = shear[pileIdx][i];
            if (shear[pileIdx][i] < minShear) minShear = shear[pileIdx][i];
        }
    }
}

int  PileFEAmodeler::getExitStatus()
{
    return 0;
}

/* **** return data for plotting or post analysis output **** */

QList<QVector<double> > *getDisplacements(int pile, int dir)
{
    QList<QVector<double> > *list = new QList<QVector<double> >;

    return list;
}

QList<QVector<double> > *getMoment(int pile)
{
    QList<QVector<double> > *list = new QList<QVector<double> >;

    return list;
}

QList<QVector<double> > *getShear(int pile)
{
    QList<QVector<double> > *list = new QList<QVector<double> >;

    return list;
}

QList<QVector<double> > *getForce(int pile)
{
    QList<QVector<double> > *list = new QList<QVector<double> >;

    return list;
}

QList<QVector<double> > *getPult(int pile)
{
    QList<QVector<double> > *list = new QList<QVector<double> >;

    return list;
}

QList<QVector<double> > *getY50(int pile)
{
    QList<QVector<double> > *list = new QList<QVector<double> >;

    return list;
}

