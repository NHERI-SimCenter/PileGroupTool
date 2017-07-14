#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include "utilWindows/copyrightdialog.h"
#include "utilWindows/dialogpreferences.h"
#include "utilWindows/dialogabout.h"

extern int getTzParam(double phi, double b, double sigV, double pEleLength, double *tult, double *z50);
extern int getQzParam(double phiDegree, double b, double sigV, double G, double *qult, double *z50);
extern int getPyParam(double pyDepth,
                      double gamma,
                      double phiDegree,
                      double b,
                      double pEleLength,
                      double puSwitch,
                      double kSwitch,
                      double gwtSwitch,
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

StandardStream sserr;
OPS_Stream *opserrPtr = &sserr;
Domain theDomain;
//SimulationInformation simulationInfo;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->fetchSettings();
    this->updateUI();

    // setup data
    numPiles = 1;
    P        = 0.0;

    L1                       = 1.0;
    L2[numPiles-1]           = 20.0;
    pileDiameter[numPiles-1] = 1.0;
    E[numPiles-1]            = 25.0e6;
    xOffset[numPiles-1]      = 0.0;

    gwtDepth = 3.00;
    gamma    = 17.0;
    phi      = 36.0;
    gSoil    = 150000;
    puSwitch = 1;
    kSwitch  = 1;
    gwtSwitch= 1;

     // set initial state of check boxes
    useToeResistance    = false;
    assumeRigidPileHeadConnection = false;

    // analysis parameters
    displacementRatio = 0.0;

    // setup table of soil layers
    QStringList matTableLabels;
    matTableLabels << "Thickness" << "Wet Unit Weight" << "Saturated Unit Weight" << "Friction Angle" << "Shear Modulus";
    ui->matTable->setRowCount(5);
    ui->matTable->setColumnCount(1);
    ui->matTable->setVerticalHeaderLabels(matTableLabels);
    ui->matTable->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
    ui->matTable->setItemPrototype(ui->matTable->item(3,0));
    ui->matTable->setItemDelegate(new QItemDelegate());
    
    // set up initial values before activating live analysis (= connecting the slots)
    //    or the program will fail in the analysis due to missing information
    setupLayers();
    reDrawTable();    

    ui->pileIndex->setValue(1);
    ui->pileIndex->setMinimum(1);
    ui->pileIndex->setMaximum(numPiles);

    int pileIdx = ui->pileIndex->value() - 1;

    ui->appliedForce->setValue(P);
    ui->pileDiameter->setValue(pileDiameter[pileIdx]);
    ui->freeLength->setValue(L1);
    ui->embeddedLength->setValue(L2[pileIdx]);
    ui->Emodulus->setValue(E[pileIdx]);

    ui->groundWaterTable->setValue(gwtDepth);

    ui->chkBox_assume_rigid_cap->setCheckState(assumeRigidPileHeadConnection?Qt::Checked:Qt::Unchecked);
    ui->chkBox_include_toe_resistance->setCheckState(useToeResistance?Qt::Checked:Qt::Unchecked);

    //connect(ui->matTable, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)), this, SLOT(updateInfo(QTableWidgetItem*)));
    connect(ui->matTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateInfo(QTableWidgetItem*)));
    //connect(ui->matTable, SIGNAL(itemEntered(QTableWidgetItem*)), this, SLOT(updateInfo(QTableWidgetItem*)));

    inSetupState = false;
      
    this->doAnalysis();
}

void MainWindow::doAnalysis(void)
{
    QVector<HEAD_NODE_TYPE> headNodeList(MAXPILES, {-1,-1,0.0, 1.0, 1.0});

    for (int k=0; k<MAXPILES; k++) {
        headNodeList[k] = {-1, -1, 0.0, 1.0, 1.0};
    }

    if (inSetupState) return;

    // clear existing model
    theDomain.clearAll();
    OPS_clearAllUniaxialMaterial();
    ops_Dt = 0.0;

    //
    // find meshing parameters
    //

    QVector<QVector<int>> elemsInLayer(MAXPILES,QVector<int>(3,minElementsPerLayer));
    QVector<double> depthOfLayer = QVector<double>(4, 0.0); // add a buffer element for bottom of the third layer

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
        numNodePile[pileIdx] = numElementsInAir+2; // free standing plus bottom plus surface node

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
            if (iLayer > 0) {
                overburdonStress = mSoilLayers[iLayer-1].getLayerBottomStress();
            }
            else {
                overburdonStress = 0.00;
            }
            mSoilLayers[iLayer].setLayerOverburdenStress(overburdonStress);

            groundWaterHead = gwtDepth - depthOfLayer[iLayer];
            mSoilLayers[iLayer].setLayerGWHead(groundWaterHead);
        }

        // add numper of nodes in this pile to global number of nodes
        numNodePiles += numNodePile[pileIdx];
    }

    /* ******** done with sizing and adjustments ******** */

    QVector<QVector<double> > locList(MAXPILES, QVector<double>(numNodePiles+1,0.0));
    QVector<QVector<double> > pultList(MAXPILES, QVector<double>(numNodePiles+1,0.0));
    QVector<QVector<double> > y50List(MAXPILES, QVector<double>(numNodePiles+1,0.0));

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
        qDebug() << "+ pile index: " << pileIdx;

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

        locList[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]]  = zCoord;
        pultList[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]] = 0.001;
        y50List[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]]  = 0.00001;

        //
        // create bottom pile node
        //
        numNode += 1;

        Node *theNode = 0;
        nodeTag = numNode+ioffset2;

        //qDebug() << "Node(" << nodeTag << "," << 6 << "," << xOffset[pileIdx] << "," << 0.0 << "," << zCoord << ")";

        theNode = new Node(nodeTag, 6, xOffset[pileIdx], 0., zCoord);  theDomain.addNode(theNode);
        if (numNode != 1) {
            SP_Constraint *theSP = 0;
            theSP = new SP_Constraint(nodeTag, 1, 0., true); theDomain.addSP_Constraint(theSP);
            theSP = new SP_Constraint(nodeTag, 3, 0., true); theDomain.addSP_Constraint(theSP);
            theSP = new SP_Constraint(nodeTag, 5, 0., true); theDomain.addSP_Constraint(theSP);
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

            theNode = new Node(numNode,         3, xOffset[pileIdx], 0., zCoord);  theDomain.addNode(theNode);
            theNode = new Node(numNode+ioffset, 3, xOffset[pileIdx], 0., zCoord);  theDomain.addNode(theNode);

            SP_Constraint *theSP = 0;
            theSP = new SP_Constraint(numNode, 0, 0., true);  theDomain.addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode, 1, 0., true);  theDomain.addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode, 2, 0., true);  theDomain.addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode+ioffset, 0, 0., true);  theDomain.addSP_Constraint(theSP); // ?
            theSP = new SP_Constraint(numNode+ioffset, 1, 0., true);  theDomain.addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode+ioffset, 2, 0., true);  theDomain.addSP_Constraint(theSP);
        }

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

                theNode = new Node(numNode,         3, xOffset[pileIdx], 0., zCoord);  theDomain.addNode(theNode);
                theNode = new Node(numNode+ioffset, 3, xOffset[pileIdx], 0., zCoord);  theDomain.addNode(theNode);

                SP_Constraint *theSP = 0;
                theSP = new SP_Constraint(numNode, 0, 0., true);  theDomain.addSP_Constraint(theSP);
                theSP = new SP_Constraint(numNode, 1, 0., true);  theDomain.addSP_Constraint(theSP);
                theSP = new SP_Constraint(numNode, 2, 0., true);  theDomain.addSP_Constraint(theSP);
                theSP = new SP_Constraint(numNode+ioffset, 1, 0., true);  theDomain.addSP_Constraint(theSP);
                theSP = new SP_Constraint(numNode+ioffset, 2, 0., true);  theDomain.addSP_Constraint(theSP);

                //
                // pile nodes
                //

                nodeTag = numNode+ioffset2;

                //qDebug() << "Node(" << nodeTag << "," << 6 << "," << xOffset[pileIdx] << "," << 0.0 << "," << zCoord << ")";

                theNode = new Node(nodeTag, 6, xOffset[pileIdx], 0., zCoord);  theDomain.addNode(theNode);
                if (numNode != 1) {
                    SP_Constraint *theSP = 0;
                    theSP = new SP_Constraint(nodeTag, 1, 0., true); theDomain.addSP_Constraint(theSP);
                    theSP = new SP_Constraint(nodeTag, 3, 0., true); theDomain.addSP_Constraint(theSP);
                    theSP = new SP_Constraint(nodeTag, 5, 0., true); theDomain.addSP_Constraint(theSP);
                }

                // constrain spring and pile nodes with equalDOF (identity constraints)
                MP_Constraint *theMP = new MP_Constraint(numNode+ioffset2, numNode+ioffset, Ccr, rcDof, rcDof);
                theDomain.addMP_Constraint(theMP);

                //
                // create soil-spring materials
                //

                // # p-y spring material
                puSwitch  = 2;  // Hanson
                kSwitch   = 1;  // API

                gwtSwitch = (gwtDepth > -zCoord)?1:2;

                int dummy = iLayer;

                double depthInLayer = -zCoord - depthOfLayer[iLayer];
                sigV = mSoilLayers[iLayer].getEffectiveStress(depthInLayer);
                phi  = mSoilLayers[iLayer].getLayerFrictionAng();

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
                pultList[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]] = 0.001;
                y50List[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]]  = 0.00001;

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
                theDomain.addElement(theEle);

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

        if (L1 > 0.01) {
            eleSize = L1 / (1.0*numElementsInAir);
            zCoord = -1.0e-3;

            while (zCoord < (L1 + 1.0e-6)) {
                numNode += 1;

                nodeTag = numNode+ioffset2;

                //qDebug() << "Node(" << nodeTag << "," << 6 << "," << xOffset[pileIdx] << "," << 0.0 << "," << zCoord << ")";

                Node *theNode = new Node(nodeTag, 6, xOffset[pileIdx], 0., zCoord);  theDomain.addNode(theNode);
                if (numNode != 1) {
                    SP_Constraint *theSP = 0;
                    theSP = new SP_Constraint(nodeTag, 1, 0., true); theDomain.addSP_Constraint(theSP);
                    theSP = new SP_Constraint(nodeTag, 3, 0., true); theDomain.addSP_Constraint(theSP);
                    theSP = new SP_Constraint(nodeTag, 5, 0., true); theDomain.addSP_Constraint(theSP);
                }

                locList[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]]  = zCoord;
                pultList[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]] = 0.001;
                y50List[pileIdx][numNode+ioffset2-nodeIDoffset[pileIdx]]  = 0.00001;

                zCoord += eleSize;
            }
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
            theDomain.addElement(theEle);
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

            getQzParam(phi, pileDiameter[pileIdx],  sigVq,  gSoil, &qult, &z50q);
            UniaxialMaterial *theMat = new QzSimple1(1+ioffset, 2, qult, z50q, 0.0, 0.0);
            OPS_addUniaxialMaterial(theMat);

            ID Onedirection(1);
            Onedirection[0] = 2;

            // pile toe
            Element *theEle = new ZeroLength(1+pileIdx+ioffset4, 3, 1, 1+ioffset, x, y, 1, &theMat, Onedirection);
            theDomain.addElement(theEle);
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

            qDebug() << "Node(" << nodeTag << "," << 6 << "," << headNodeList[pileIdx].x << "," << 0.0 << "," << L1 << ")";

            Node *theNode = new Node(nodeTag, 6, headNodeList[pileIdx].x, 0., L1);  theDomain.addNode(theNode);

            // create single point constraints
            if (assumeRigidPileHeadConnection) {
                // constrain spring and pile nodes with equalDOF (identity constraints)
                static Matrix Chr (6, 6);
                Chr.Zero();
                Chr(0,0)=1.0; Chr(1,1)=1.0; Chr(2,2)=1.0; Chr(3,3)=1.0; Chr(4,4)=1.0; Chr(5,5)=1.0;
                static ID hcDof (6);
                hcDof(0) = 0; hcDof(1) = 1; hcDof(2) = 2; hcDof(3) = 3; hcDof(4) = 4; hcDof(5) = 5;

                qDebug() << "MP_Constraint(" << nodeTag << "," << headNodeList[pileIdx].nodeIdx << "," << "Idty(6,6)" << "," << "{0,1,2,3,4,5}" << "," << "{0,1,2,3,4,5}" << ")";

                MP_Constraint *theMP = new MP_Constraint(nodeTag, headNodeList[pileIdx].nodeIdx, Chr, hcDof, hcDof);
                theDomain.addMP_Constraint(theMP);
            }
            else {
                // constrain spring and pile nodes with equalDOF (identity constraints)
                static Matrix Chl (5, 5);
                Chl.Zero();
                Chl(0,0)=1.0; Chl(1,1)=1.0; Chl(2,2)=1.0; Chl(3,3)=1.0; Chl(4,4)=1.0;
                static ID hlDof (5);
                hlDof(0) = 0; hlDof(1) = 1; hlDof(2) = 2; hlDof(3) = 3; hlDof(4) = 5;

                qDebug() << "MP_Constraint(" << nodeTag << "," << headNodeList[pileIdx].nodeIdx << "," << "Idty(5,5)" << "," << "{0,1,2,3,5}" << "," << "{0,1,2,3,5}" << ")";

                MP_Constraint *theMP = new MP_Constraint(nodeTag, headNodeList[pileIdx].nodeIdx, Chl, hlDof, hlDof);
                theDomain.addMP_Constraint(theMP);
            }

            // create beams for pile head
            if (prevNode > 0) {
                numElem++;

                qDebug() << "DispBeamColumn3d(" << numElem << "," << prevNode << "," << numNode << ")";

                Element *theEle = new DispBeamColumn3d(numElem, prevNode, nodeTag,
                                                       3, theSections, *theIntegration, *theTransformation);
                theDomain.addElement(theEle);
            }

            prevNode = nodeTag;
        }
    }

    if (numPiles == 1) {
        /* need extra fixities to prevent singular system matrix */
        int nodeTag = numNode + ioffset5;

        SP_Constraint *theSP = 0;
        theSP = new SP_Constraint(nodeTag, 4, 0., true); theDomain.addSP_Constraint(theSP);
    }

    int numLoadedNode = headNodeList[0].nodeIdx;

    /* *** done with the pile head *** */

    if (numNode != numNodePiles) {
        qDebug() << "ERROR: " << numNode << " nodes generated but " << numNodePiles << "expected" << endln;
    }

    //
    // create load pattern and add loads
    //

    LinearSeries *theTimeSeries = new LinearSeries(1, 1.0);
    LoadPattern *theLoadPattern = new LoadPattern(1);
    theLoadPattern->setTimeSeries(theTimeSeries);
    static Vector load(6); load.Zero(); load(0) = P;
    NodalLoad *theLoad = new NodalLoad(0, numLoadedNode, load);
    theLoadPattern->addNodalLoad(theLoad);
    theDomain.addLoadPattern(theLoadPattern);

    //
    // create the analysis
    //

    AnalysisModel     *theModel = new AnalysisModel();
    CTestNormDispIncr *theTest = new CTestNormDispIncr(1.0e-3, 20, 0);
    EquiSolnAlgo      *theSolnAlgo = new NewtonRaphson();
    StaticIntegrator  *theIntegrator = new LoadControl(0.05, 1, 0.05, 0.05);
    //ConstraintHandler *theHandler = new TransformationConstraintHandler();
    ConstraintHandler *theHandler = new PenaltyConstraintHandler(1.0e14, 1.0e14);
    RCM               *theRCM = new RCM();
    DOF_Numberer      *theNumberer = new DOF_Numberer(*theRCM);
    BandGenLinSolver  *theSolver = new BandGenLinLapackSolver();
    LinearSOE         *theSOE = new BandGenLinSOE(*theSolver);

    StaticAnalysis    theAnalysis(theDomain,
                                  *theHandler,
                                  *theNumberer,
                                  *theModel,
                                  *theSolnAlgo,
                                  *theSOE,
                                  *theIntegrator);
    theSolnAlgo->setConvergenceTest(theTest);

    //
    //analyze & get results
    //
    theAnalysis.analyze(20);
    theDomain.calculateNodalReactions(0);

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
    //double maxStress = 0.0;
    //double minStress = 0.0;

    int pileIdx;

    for (pileIdx=0; pileIdx<numPiles; pileIdx++) {

        for (int i=1; i<=numNodePile[pileIdx]; i++) {
            //zero[i-1] = 0.0;

            //qDebug() << "getNode(" << i+nodeIDoffset[pileIdx] << ")";

            Node *theNode = theDomain.getNode(i+nodeIDoffset[pileIdx]);
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

            Element *theEle = theDomain.getElement(i+elemIDoffset[pileIdx]);
            const Vector &eleForces = theEle->getResistingForce();
            moment[pileIdx][i] = eleForces(10);
            if (moment[pileIdx][i] > maxMoment) maxMoment = moment[pileIdx][i];
            if (moment[pileIdx][i] < minMoment) minMoment = moment[pileIdx][i];
            shear[pileIdx][i] = eleForces(6);
            if (shear[pileIdx][i] > maxShear) maxShear = shear[pileIdx][i];
            if (shear[pileIdx][i] < minShear) minShear = shear[pileIdx][i];
        }
    }

    //
    // plot results
    //

    // plot displacemenet
    if (showDisplacements) {
        ui->displPlot->clearPlottables();

        ui->displPlot->autoAddPlottableToLegend();
        ui->displPlot->legend->setVisible(true);
        ui->displPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

        ui->displPlot->addGraph();
        ui->displPlot->graph(0)->setPen(QPen(Qt::black));
        ui->displPlot->graph(0)->setData(zero,loc[0]);
        ui->displPlot->graph(0)->removeFromLegend();

        for (pileIdx=0; pileIdx<numPiles; pileIdx++) {

            QCPCurve *dispCurve = new QCPCurve(ui->displPlot->xAxis, ui->displPlot->yAxis);
            dispCurve->setData(disp[pileIdx].mid(0,numNodePile[pileIdx]),loc[pileIdx].mid(0,numNodePile[pileIdx]));
            dispCurve->setPen(QPen(LINE_COLOR[pileIdx], 3));
            dispCurve->setName(QString("Pile #%1").arg(pileIdx+1));
            ui->displPlot->addPlottable(dispCurve);
        }

        ui->displPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        ui->displPlot->axisRect()->setupFullAxesBox();
        ui->displPlot->rescaleAxes();
        ui->displPlot->replot();
    }

    // plot shear
    if (showShear) {
        ui->shearPlot->clearPlottables();

        ui->shearPlot->autoAddPlottableToLegend();
        ui->shearPlot->legend->setVisible(true);
        ui->shearPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

        ui->shearPlot->addGraph();
        ui->shearPlot->graph(0)->setData(zero,loc[0]);
        ui->shearPlot->graph(0)->setPen(QPen(Qt::black));
        ui->shearPlot->graph(0)->removeFromLegend();

        for (pileIdx=0; pileIdx<numPiles; pileIdx++) {

            QCPCurve *shearCurve = new QCPCurve(ui->shearPlot->xAxis, ui->shearPlot->yAxis);
            shearCurve->setData(shear[pileIdx].mid(0,numNodePile[pileIdx]),loc[pileIdx].mid(0,numNodePile[pileIdx]));
            shearCurve->setPen(QPen(LINE_COLOR[pileIdx], 3));
            shearCurve->setName(QString("Pile #%1").arg(pileIdx+1));
            ui->shearPlot->addPlottable(shearCurve);
        }

        ui->shearPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        ui->shearPlot->axisRect()->setupFullAxesBox();
        ui->shearPlot->rescaleAxes();
        ui->shearPlot->replot();
    }

    // plot moments
    if (showMoments) {
        ui->momentPlot->clearPlottables();

        ui->momentPlot->autoAddPlottableToLegend();
        ui->momentPlot->legend->setVisible(true);
        ui->momentPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

        ui->momentPlot->addGraph();
        ui->momentPlot->graph(0)->setData(zero,loc[0]);
        ui->momentPlot->graph(0)->setPen(QPen(Qt::black));
        ui->momentPlot->graph(0)->removeFromLegend();

        for (pileIdx=0; pileIdx<numPiles; pileIdx++) {
            QCPCurve *momCurve = new QCPCurve(ui->momentPlot->xAxis, ui->momentPlot->yAxis);
            momCurve->setData(moment[pileIdx].mid(0,numNodePile[pileIdx]),loc[pileIdx].mid(0,numNodePile[pileIdx]));
            momCurve->setPen(QPen(LINE_COLOR[pileIdx], 3));
            momCurve->setName(QString("Pile #%1").arg(pileIdx+1));
            ui->momentPlot->addPlottable(momCurve);
        }

        ui->momentPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        ui->momentPlot->axisRect()->setupFullAxesBox();
        ui->momentPlot->rescaleAxes();
        ui->momentPlot->replot();
    }

    // plot vertical stress
    if (showStress) {
        ui->stressPlot->clearPlottables();

        ui->stressPlot->autoAddPlottableToLegend();
        ui->stressPlot->legend->setVisible(true);
        ui->stressPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);

        ui->stressPlot->addGraph();
        ui->stressPlot->graph(0)->setData(zero,loc[0]);
        ui->stressPlot->graph(0)->setPen(QPen(Qt::black));
        ui->stressPlot->graph(0)->removeFromLegend();

        for (pileIdx=0; pileIdx<numPiles; pileIdx++) {
            QCPCurve *stressCurve = new QCPCurve(ui->stressPlot->xAxis, ui->stressPlot->yAxis);
            stressCurve->setData(stress[pileIdx],loc[pileIdx]);
            stressCurve->setPen(QPen(LINE_COLOR[pileIdx], 3));
            stressCurve->setName(QString("Pile #%1").arg(pileIdx+1));
            ui->stressPlot->addPlottable(stressCurve);
        }

        ui->stressPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        ui->stressPlot->axisRect()->setupFullAxesBox();
        ui->stressPlot->rescaleAxes();
        ui->stressPlot->replot();
    }

    // plot p_ultimate
    if (showPultimate) {
        ui->pultPlot->clearPlottables();

        ui->pultPlot->autoAddPlottableToLegend();
        ui->pultPlot->legend->setVisible(true);

        ui->pultPlot->addGraph();
        ui->pultPlot->graph(0)->setData(zero,loc[0]);
        ui->pultPlot->graph(0)->setPen(QPen(Qt::black));
        ui->pultPlot->graph(0)->removeFromLegend();

        for (pileIdx=0; pileIdx<numPiles; pileIdx++) {
            QCPCurve *pultCurve = new QCPCurve(ui->pultPlot->xAxis, ui->pultPlot->yAxis);
            pultCurve->setData(pultList[pileIdx].mid(0,numNodePile[pileIdx]),locList[pileIdx].mid(0,numNodePile[pileIdx]));
            pultCurve->setPen(QPen(LINE_COLOR[pileIdx], 3));
            pultCurve->setName(QString("Pile #%1").arg(pileIdx+1));
            ui->pultPlot->addPlottable(pultCurve);
        }

        ui->pultPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        ui->pultPlot->axisRect()->setupFullAxesBox();
        ui->pultPlot->rescaleAxes();
        ui->pultPlot->replot();
    }

    // plot y_50
    if (showY50) {
        ui->y50Plot->clearPlottables();

        ui->y50Plot->autoAddPlottableToLegend();
        ui->y50Plot->legend->setVisible(true);

        ui->y50Plot->addGraph();
        ui->y50Plot->graph(0)->setData(zero,loc[0]);
        ui->y50Plot->graph(0)->setPen(QPen(Qt::black));
        ui->y50Plot->graph(0)->removeFromLegend();

        for (pileIdx=0; pileIdx<numPiles; pileIdx++) {
            QCPCurve *y50Curve = new QCPCurve(ui->y50Plot->xAxis, ui->y50Plot->yAxis);
            y50Curve->setData(y50List[pileIdx].mid(0,numNodePile[pileIdx]),locList[pileIdx].mid(0,numNodePile[pileIdx]));
            y50Curve->setPen(QPen(LINE_COLOR[pileIdx], 3));
            y50Curve->setName(QString("Pile #%1").arg(pileIdx+1));
            ui->y50Plot->addPlottable(y50Curve);
        }

        ui->y50Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        ui->y50Plot->axisRect()->setupFullAxesBox();
        ui->y50Plot->rescaleAxes();
        ui->y50Plot->replot();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fetchSettings()
{
    if (settings != NULL) { delete settings; }
    settings = new QSettings("NHERI SimCenter","Pile Group Tool");

    // viewer settings
    settings->beginGroup("viewer");
        showDisplacements = settings->value("displacements",1).toBool();
        showMoments       = settings->value("moments",1).toBool();
        showShear         = settings->value("shear",1).toBool();
        showStress        = settings->value("stress",1).toBool();
        showPultimate     = settings->value("pult",1).toBool();
        showY50           = settings->value("compliance",1).toBool();
    settings->endGroup();

    // meshing parameters
    settings->beginGroup("fea");
        minElementsPerLayer = settings->value("minElemementsPerLayer",15).toInt();
        maxElementsPerLayer = settings->value("maxElemementsPerLayer",40).toInt();
        numElementsInAir    = settings->value("numElementsInAir", 4).toInt();
    settings->endGroup();

    /****** end of settings ******/
}

/* ***** menu actions ***** */

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

/* ***** check box status changes ***** */

void MainWindow::on_chkBox_assume_rigid_cap_clicked(bool checked)
{
    assumeRigidPileHeadConnection = checked;

    this->doAnalysis();
}

void MainWindow::on_chkBox_include_toe_resistance_clicked(bool checked)
{
    useToeResistance = checked;

    this->doAnalysis();
}

/* ***** loading parameter changes ***** */

void MainWindow::on_appliedForce_valueChanged(double arg1)
{
    //P = ui->appliedForce->value();
    P = arg1;

    int sliderPosition = nearbyint(100.*P/5000.0);
    if (sliderPosition >  100) sliderPosition= 100;
    if (sliderPosition < -100) sliderPosition=-100;
    ui->displacementSlider->setValue(sliderPosition);
    //qDebug() << "valueChanged:: Force value: " << P << ",  sliderPosition: " << sliderPosition << endln;

    this->doAnalysis();
}

void MainWindow::on_appliedForce_editingFinished()
{
    P = ui->appliedForce->value();

    int sliderPosition = nearbyint(100.*P/5000.0);
    if (sliderPosition >  100) sliderPosition= 100;
    if (sliderPosition < -100) sliderPosition=-100;

    //qDebug() << "editingFinished:: Force value: " << P << ",  sliderPosition: " << sliderPosition << endln;
    ui->displacementSlider->setSliderPosition(sliderPosition);

    this->doAnalysis();
}

void MainWindow::on_displacementSlider_valueChanged(int value)
{
    // slider moved -- the number of steps (100) is a parameter to the slider in mainwindow.ui
    displacementRatio = double(value)/100.0;

    P = 5000.0 * displacementRatio;

    //qDebug() << "Force value: " << P << ",  sliderPosition: " << value << endln;
    ui->appliedForce->setValue(P);

    this->doAnalysis();
}

/* ***** analysis parameter changes ***** */

void MainWindow::on_pileDiameter_valueChanged(double arg1)
{
    int pileIdx = ui->pileIndex->value() - 1;

    pileDiameter[pileIdx] = arg1;
    this->doAnalysis();
}

void MainWindow::on_embeddedLength_valueChanged(double arg1)
{
    int pileIdx = ui->pileIndex->value() - 1;

    L2[pileIdx] = arg1;
    this->doAnalysis();
}

void MainWindow::on_freeLength_valueChanged(double arg1)
{
    L1 = arg1;
    this->doAnalysis();
}

void MainWindow::on_Emodulus_valueChanged(double arg1)
{
    int pileIdx = ui->pileIndex->value() - 1;

    E[pileIdx] = arg1;
    this->doAnalysis();
}

void MainWindow::on_groundWaterTable_valueChanged(double arg1)
{
    gwtDepth = arg1;
    this->doAnalysis();
}


/* ***** soil properties table functions ***** */

void MainWindow::setupLayers()
{
    mSoilLayers.clear();
    mSoilLayers.push_back(soilLayer("Layer 1", 3.0, 15.0, 18.0, 2.0e5, 30, QColor(100,0,0,100)));
    mSoilLayers.push_back(soilLayer("Layer 2", 3.0, 16.0, 19.0, 2.0e5, 35, QColor(0,100,0,100)));
    mSoilLayers.push_back(soilLayer("Layer 3", 4.0, 14.0, 17.0, 2.0e5, 25, QColor(0,0,100,100)));

    // set the GWT depth for each layer
    int numLayers = mSoilLayers.size();
    double layerDepthFromSurface = 0.0;
    for (int ii = 0; ii < numLayers; ii++)
    {
        mSoilLayers[ii].setGWTdepth(gwtDepth - layerDepthFromSurface);
        layerDepthFromSurface += mSoilLayers[ii].getLayerThickness();
        if (ii > 0)
            mSoilLayers[ii].setLayerTopStress(mSoilLayers[ii-1].getLayerBottomStress());
    }
}

void MainWindow::reDrawTable()
{
  // update and redraw the ui->matTable
    int numLayers = mSoilLayers.size();
    QStringList matTableHeaders;
    ui->matTable->setColumnCount(numLayers);
    for (int ii = 0; ii < numLayers; ii++)
    {
        matTableHeaders << mSoilLayers[ii].getLayerName();
        ui->matTable->setItem(0, ii, new QTableWidgetItem(QString::number(mSoilLayers[ii].getLayerThickness())));
        ui->matTable->setItem(1, ii, new QTableWidgetItem(QString::number(mSoilLayers[ii].getLayerUnitWeight())));
        ui->matTable->setItem(2, ii, new QTableWidgetItem(QString::number(mSoilLayers[ii].getLayerSatUnitWeight())));
        ui->matTable->setItem(3, ii, new QTableWidgetItem(QString::number(mSoilLayers[ii].getLayerFrictionAng())));
        ui->matTable->setItem(4, ii, new QTableWidgetItem(QString::number(mSoilLayers[ii].getLayerStiffness())));
    }

    ui->matTable->setHorizontalHeaderLabels(matTableHeaders);

    for (int ii = 0; ii < ui->matTable->rowCount(); ii++)
	for (int jj = 0; jj < ui->matTable->columnCount(); jj++)
		ui->matTable->item(ii,jj)->setTextAlignment(Qt::AlignHCenter);
}

void MainWindow::updateInfo(QTableWidgetItem * item)
{
    //if (item && item == ui->matTable->currentItem()) {
        if(item->row() == 0) {
            double thickness = item->text().toDouble();
            if (thickness < 0.10) item->setText("0.10");
            mSoilLayers[item->column()].setLayerThickness(item->text().toDouble());
        }
        else if (item->row() == 1) {
            mSoilLayers[item->column()].setLayerUnitWeight(item->text().toDouble());
        }
        else if (item->row() == 2) {
            mSoilLayers[item->column()].setLayerSatUnitWeight(item->text().toDouble());
        }
        else if (item->row() == 3) {
            mSoilLayers[item->column()].setLayerFrictionAng(item->text().toDouble());
        }
        else if (item->row() == 4) {
            mSoilLayers[item->column()].setLayerStiffness(item->text().toDouble());
        }
    //}

    this->doAnalysis();
}


void MainWindow::on_action_Open_triggered()
{

}

void MainWindow::on_actionExport_to_OpenSees_triggered()
{

}

void MainWindow::on_actionReset_triggered()
{

}

void MainWindow::on_actionFEA_parameters_triggered()
{

}

void MainWindow::on_actionLicense_Information_triggered()
{
    CopyrightDialog *dlg = new CopyrightDialog(this);
    dlg->exec();
}

void MainWindow::on_btn_deletePile_clicked()
{
    int pileIdx = ui->pileIndex->value() - 1;
    if (pileIdx < numPiles && numPiles > 1) {
        if ( pileIdx < numPiles ) {
            for (int j=pileIdx; j<numPiles; j++) {
                L2[j-1]           = L2[j];
                pileDiameter[j-1] = pileDiameter[j];
                E[j-1]            = E[j];
                xOffset[j-1]      = xOffset[j];
            }
        }
        numPiles--;
        while (pileIdx >= numPiles) pileIdx--;

        ui->pileIndex->setMaximum(numPiles);
        ui->pileIndex->setValue(pileIdx+1);

        this->doAnalysis();
    }
}

void MainWindow::on_btn_newPile_clicked()
{
    if (numPiles < MAXPILES) {
        L2[numPiles]           = L2[numPiles-1];
        pileDiameter[numPiles] = pileDiameter[numPiles-1];
        E[numPiles]            = E[numPiles-1];
        xOffset[numPiles]      = xOffset[numPiles-1] + 2.0*pileDiameter[numPiles-1];
        numPiles++;
    }
    else
    {
        QMessageBox msg(QMessageBox::Information,"Info","Maximum number of piles reached.");
        msg.exec();
    }

    ui->pileIndex->setMaximum(numPiles);
    ui->pileIndex->setValue(numPiles);

    this->doAnalysis();
}

void MainWindow::on_xOffset_valueChanged(double arg1)
{
    int pileIdx = ui->pileIndex->value() - 1;
    xOffset[pileIdx] = ui->xOffset->value();

    this->doAnalysis();
}

void MainWindow::on_pileIndex_valueChanged(int arg1)
{
    int pileIdx = ui->pileIndex->value() - 1;
    ui->pileDiameter->setValue(pileDiameter[pileIdx]);
    ui->Emodulus->setValue(E[pileIdx]);
    ui->embeddedLength->setValue(L2[pileIdx]);
    ui->freeLength->setValue(L1);
    ui->xOffset->setValue(xOffset[pileIdx]);

    // refresh graphics

}

void MainWindow::on_action_About_triggered()
{
    DialogAbout *dlg = new DialogAbout();
    dlg->exec();
    delete dlg;
}

void MainWindow::on_actionPreferences_triggered()
{
    DialogPreferences *dlg = new DialogPreferences(this, settings);
    dlg->exec();
    delete dlg;

    this->fetchSettings();
    this->updateUI();
    this->doAnalysis();
}

void MainWindow::updateUI()
{
    if (!showDisplacements && ui->tabWidget->indexOf(ui->displacement)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->displacement));
    }
    if (!showMoments && ui->tabWidget->indexOf(ui->moment)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->moment));
    }
    if (!showShear && ui->tabWidget->indexOf(ui->shear)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->shear));
    }
    if (!showStress && ui->tabWidget->indexOf(ui->stress)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->stress));
    }
    if (!showPultimate && ui->tabWidget->indexOf(ui->pult)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->pult));
    }
    if (!showY50 && ui->tabWidget->indexOf(ui->y50)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->y50));
    }

    int numTabs = ui->tabWidget->count();

    if (showDisplacements && ui->tabWidget->indexOf(ui->displacement) < 0 ) {
        ui->tabWidget->addTab(ui->displacement,"Displacement");
    }
    if (showMoments && ui->tabWidget->indexOf(ui->moment) < 0 ) {
        ui->tabWidget->addTab(ui->moment,"Moment");
    }
    if (showShear && ui->tabWidget->indexOf(ui->shear) < 0 ) {
        ui->tabWidget->addTab(ui->shear,"Shear");
    }
    if (showStress && ui->tabWidget->indexOf(ui->stress) < 0 ) {
        ui->tabWidget->addTab(ui->stress,"Stress");
    }
    if (showPultimate && ui->tabWidget->indexOf(ui->pult) < 0 ) {
        ui->tabWidget->addTab(ui->pult,"p_ult");
    }
    if (showY50 && ui->tabWidget->indexOf(ui->y50) < 0) {
        ui->tabWidget->addTab(ui->y50,"y50");
    }
}
