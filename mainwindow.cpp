#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    // setup data
    P        = 0.0;
    L1       = 1.0;
    L2       = 20.0;
    pileDiameter = 1.0;
    gwtDepth = 3.00;
    E        = 25.0e6;
    numEle   = 80;
    gamma    = 17.0;
    phi      = 36.0;
    gSoil    = 150000;
    puSwitch = 1;
    kSwitch  = 1;
    gwtSwitch= 1;

     // set initial state of check boxes
    useToeResistance    = false;
    assumeRigidPileHead = false;

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

    ui->appliedForce->setValue(P);
    ui->pileDiameter->setValue(pileDiameter);
    ui->freeLength->setValue(L1);
    ui->embeddedLength->setValue(L2);
    ui->Emodulus->setValue(E);
    ui->groundWaterTable->setValue(gwtDepth);

    ui->chkBox_assume_rigid_cap->setCheckState(assumeRigidPileHead?Qt::Checked:Qt::Unchecked);
    ui->chkBox_include_toe_resistance->setCheckState(useToeResistance?Qt::Checked:Qt::Unchecked);

    connect(ui->matTable, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)),
          this, SLOT(updateInfo(QTableWidgetItem*)));
    connect(ui->matTable, SIGNAL(itemChanged(QTableWidgetItem*)),
          this, SLOT(updateInfo(QTableWidgetItem*)));

    inSetupState = false;
      
    this->doAnalysis();
}

void MainWindow::doAnalysis(void)
{
    if (inSetupState) return;

    // clear existing model
    theDomain.clearAll();
    OPS_clearAllUniaxialMaterial();
    ops_Dt = 0.0;

    //
    // find meshing parameters
    //
    //
    QVector<int>    elemsInLayer = QVector<int>(3,minElementsPerLayer);
    QVector<double> depthOfLayer = QVector<double>(4, 0.0); // add a buffer element for bottom of the third layer

    int numNodePile = numElementsInAir+1;

    int maxLayers=3;

    for (int iLayer=0; iLayer < maxLayers; iLayer++)
    {
        if (depthOfLayer[iLayer] >= L2) {
            maxLayers = iLayer;
            break;
        }

        double thickness = mSoilLayers[iLayer].getLayerThickness();

        int numElemThisLayer = int(thickness/pileDiameter);
        if (numElemThisLayer < minElementsPerLayer) numElemThisLayer = minElementsPerLayer;
        if (numElemThisLayer > maxElementsPerLayer) numElemThisLayer = maxElementsPerLayer;
        numElemThisLayer += 1; // accounting for the shift of elements such that no spring ends up on an interface

        // remember number of elements in this layer
        elemsInLayer[iLayer] = numElemThisLayer;

        // total node count
        numNodePile += numElemThisLayer;

        // compute bottom of this layer/top of the next layer
        depthOfLayer[iLayer+1] = depthOfLayer[iLayer] + thickness;

        // check if layer ends below the pile toe
        if (depthOfLayer[iLayer+1] > L2) {
            thickness = L2 - depthOfLayer[iLayer];
            mSoilLayers[iLayer].setLayerThickness(thickness);
            depthOfLayer[iLayer+1] = depthOfLayer[iLayer] + thickness;
        }

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

    // make sure we have enough soil for the analysis
    if (L2 > depthOfLayer[maxLayers]) {
        //
        // increase thickness of the bottom layer
        //
        double thickness = L2 - depthOfLayer[maxLayers-1];
        mSoilLayers[maxLayers-1].setLayerThickness(thickness);
        depthOfLayer[maxLayers] = L2;
    }

    int ioffset  = numNodePile;
    int ioffset2 = 2*numNodePile;

    // offset for spring node generation
    int springNodeNumber = numNodePile;

    //
    // compute pile properties (compute once; used for all pile elements)
    //
    double PI = 3.14159;
    double A  = 0.2500 * PI * pileDiameter * pileDiameter;
    double Iz = 0.0625 *  A * pileDiameter * pileDiameter;
    double G  = E/(2.0*(1.+0.3));
    double J  = 1.0e10;

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
    UniaxialMaterial *theMaterials[2];
    ID direction(2);
    direction[0] = 0;
    direction[1] = 2;

    //
    // Ready to generate the structure
    //

    /* embedded pile portion */

    int numNode;

    numNode = 0;

    zCoord = -L2;

    for (int iLayer=maxLayers-1; iLayer >= 0; iLayer--)
    {
        eleSize = mSoilLayers[iLayer].getLayerThickness()/(1.0*elemsInLayer[iLayer]);
        int numNodesLayer = elemsInLayer[iLayer];

        //
        // pile only has interface node
        //
        numNode += 1;

        Node *theNode = 0;
        int nodeTag = numNode+ioffset2;
        theNode = new Node(nodeTag, 6, 0., 0., zCoord);  theDomain.addNode(theNode);
        if (numNode != 1) {
            SP_Constraint *theSP = 0;
            theSP = new SP_Constraint(nodeTag, 1, 0., true); theDomain.addSP_Constraint(theSP);
            theSP = new SP_Constraint(nodeTag, 3, 0., true); theDomain.addSP_Constraint(theSP);
            theSP = new SP_Constraint(nodeTag, 5, 0., true); theDomain.addSP_Constraint(theSP);
        }

        //
        // create spring nodes
        //
        zCoord += 0.5*eleSize;

        for (int i=2; i<=numNodesLayer; i++) {
            numNode += 1;

            //
            // spring nodes
            //

            Node *theNode = 0;

            theNode = new Node(numNode,         3, 0., 0., zCoord);  theDomain.addNode(theNode);
            theNode = new Node(numNode+ioffset, 3, 0., 0., zCoord);  theDomain.addNode(theNode);

            SP_Constraint *theSP = 0;
            theSP = new SP_Constraint(numNode, 0, 0., true);  theDomain.addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode, 1, 0., true);  theDomain.addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode, 2, 0., true);  theDomain.addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode+ioffset, 1, 0., true);  theDomain.addSP_Constraint(theSP);
            theSP = new SP_Constraint(numNode+ioffset, 2, 0., true);  theDomain.addSP_Constraint(theSP);

            //
            // pile nodes
            //

            int nodeTag = numNode+ioffset2;
            theNode = new Node(nodeTag, 6, 0., 0., zCoord);  theDomain.addNode(theNode);
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

            gwtSwitch = (gwtDepth > -zCoord)?0:1;

            double depthInLayer = -zCoord - depthOfLayer[iLayer];
            sigV = mSoilLayers[iLayer].getEffectiveStress(depthInLayer);

            UniaxialMaterial *theMat;
            getPyParam(-zCoord, sigV, phi, pileDiameter, eleSize, puSwitch, kSwitch, gwtSwitch, &pult, &y50);
            theMat = new PySimple1(numNode, 0, 2, pult, y50, 0.0, 0.0);
            OPS_addUniaxialMaterial(theMat);

            // t-z spring material
            getTzParam(phi, pileDiameter,  sigV,  eleSize, &tult, &z50);
            theMat = new TzSimple1(numNode+ioffset, 0, 2, tult, z50, 0.0);
            OPS_addUniaxialMaterial(theMat);

            //
            // create soil spring elements
            //

            theMaterials[0] = OPS_getUniaxialMaterial(i);
            theMaterials[1] = OPS_getUniaxialMaterial(i+ioffset);
            Element *theEle = new ZeroLength(numNode+1000, 3, numNode, numNode+ioffset, x, y, 2, theMaterials, direction);
            theDomain.addElement(theEle);

            zCoord += eleSize;
        }
        // climb to layer interface
        zCoord += 0.5*eleSize;

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

        while (zCoord < L1) {
            numNode += 1;

            int nodeTag = numNode+ioffset2;
            Node *theNode = new Node(nodeTag, 6, 0., 0., zCoord);  theDomain.addNode(theNode);
            if (numNode != 1) {
                SP_Constraint *theSP = 0;
                theSP = new SP_Constraint(nodeTag, 1, 0., true); theDomain.addSP_Constraint(theSP);
                theSP = new SP_Constraint(nodeTag, 3, 0., true); theDomain.addSP_Constraint(theSP);
                theSP = new SP_Constraint(nodeTag, 5, 0., true); theDomain.addSP_Constraint(theSP);
            }

            zCoord += eleSize;
        }
    }

    if (numNode != numNodePile) {
        qDebug() << "ERROR: " << numNode << " nodes generated but " << numNodePile << "expected" << endln;
    }

    //
    // create pile elements
    //

    static Vector crdV(3); crdV(0)=0.; crdV(1)=-1; crdV(2) = 0.;
    CrdTransf *theTransformation = new LinearCrdTransf3d(1, crdV);

    numEle = numNodePile-1;

    for (int i=1; i<=numEle; i++) {
        BeamIntegration *theIntegration = new LegendreBeamIntegration();
        SectionForceDeformation *theSections[3];
        SectionForceDeformation *theSection = new ElasticSection3d(1, E, A, Iz, Iz, G, J);
        theSections[0] = theSection;
        theSections[1] = theSection;
        theSections[2] = theSection;
        Element *theEle = new DispBeamColumn3d(i+ioffset2, i+ioffset2, i+ioffset2+1, 3, theSections, *theIntegration, *theTransformation);
        theDomain.addElement(theEle);
        delete theSection;
        delete theIntegration;
    }

    //
    // constrain pile cap, if requested
    //
    if (assumeRigidPileHead) {
        SP_Constraint *theSP = 0;
        theSP = new SP_Constraint(numNode+ioffset2, 4, 0., true);
        theDomain.addSP_Constraint(theSP);
    }

    //
    // set up toe resistance, if requested
    //
    if (useToeResistance) {

        // # q-z spring material
        // # vertical effective stress at pile tip, no water table (depth is embedded pile length)
        double sigVq  = mSoilLayers[maxLayers-1].getLayerBottomStress();

        getQzParam(phi, pileDiameter,  sigVq,  gSoil, &qult, &z50q);
        UniaxialMaterial *theMat = new QzSimple1(1+ioffset, 2, qult, z50q, 0.0, 0.0);
        OPS_addUniaxialMaterial(theMat);

        // pile toe
        theMaterials[0] = OPS_getUniaxialMaterial(1);
        theMaterials[1] = OPS_getUniaxialMaterial(1+ioffset);
        Element *theEle = new ZeroLength(10001, 3, 1, 1+ioffset, x, y, 2, theMaterials, direction);
        theDomain.addElement(theEle);
    }

    //
    // create load pattern and add loads
    //

    LinearSeries *theTimeSeries = new LinearSeries(1, 1.0);
    LoadPattern *theLoadPattern = new LoadPattern(1);
    theLoadPattern->setTimeSeries(theTimeSeries);
    static Vector load(6); load.Zero(); load(0) = P;
    NodalLoad *theLoad = new NodalLoad(0, numNodePile+ioffset2, load);
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


    QVector<double> loc(numNodePile);
    QVector<double> disp(numNodePile);
    QVector<double> moment(numNodePile);
    QVector<double> shear(numNodePile);
    QVector<double> zero(numNodePile);


    double maxDisp   = 0.0;
    double minDisp   = 0.0;
    double maxMoment = 0.0;
    double minMoment = 0.0;

    for (int i=1; i<=numNodePile; i++) {
        zero[i-1] = 0.0;
        Node *theNode = theDomain.getNode(i+ioffset2);
        const Vector &nodeCoord = theNode->getCrds();
        loc[i-1] = nodeCoord(2)-L2;
        const Vector &nodeDisp = theNode->getDisp();
        disp[i-1] = nodeDisp(0);
        if (disp[i-1] > maxDisp) maxDisp = disp[i-1];
        if (disp[i-1] < minDisp) minDisp = disp[i-1];
    }


    for (int i=1; i<=numEle; i++) {
        Element *theEle = theDomain.getElement(i+ioffset2);
        const Vector &eleForces = theEle->getResistingForce();
        moment[i] = eleForces(10);
        if (moment[i] > maxMoment) maxMoment = moment[i];
        if (moment[i] < minMoment) minMoment = moment[i];
        shear[i] = eleForces(6);
    }

    // plot results
    // plot displacemenet
    QCPCurve *dispCurve = new QCPCurve(ui->displPlot->xAxis, ui->displPlot->yAxis);
    dispCurve->setData(disp,loc);
    dispCurve->setPen(QPen(Qt::blue));
    ui->displPlot->clearPlottables();
    ui->displPlot->addGraph();
    ui->displPlot->graph(0)->setData(zero,loc);
    ui->displPlot->graph(0)->setPen(QPen(Qt::black));
    ui->displPlot->addPlottable(dispCurve);
    ui->displPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->displPlot->axisRect()->setupFullAxesBox();
    ui->displPlot->rescaleAxes();
    ui->displPlot->replot();


    QCPCurve *shearCurve = new QCPCurve(ui->shearPlot->xAxis, ui->shearPlot->yAxis);
    shearCurve->setData(shear,loc);
    shearCurve->setPen(QPen(Qt::blue));
    ui->shearPlot->clearPlottables();
    ui->shearPlot->addGraph();
    ui->shearPlot->graph(0)->setData(zero,loc);
    ui->shearPlot->graph(0)->setPen(QPen(Qt::black));
    ui->shearPlot->addPlottable(shearCurve);
    ui->shearPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->shearPlot->axisRect()->setupFullAxesBox();
    ui->shearPlot->rescaleAxes();
    ui->shearPlot->replot();

    QCPCurve *momCurve = new QCPCurve(ui->momentPlot->xAxis, ui->momentPlot->yAxis);
    momCurve->setData(moment,loc);
    momCurve->setPen(QPen(Qt::blue));
    ui->momentPlot->clearPlottables();
    ui->momentPlot->addGraph();
    ui->momentPlot->graph(0)->setData(zero,loc);
    ui->momentPlot->graph(0)->setPen(QPen(Qt::black));
    ui->momentPlot->addPlottable(momCurve);
    ui->momentPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->momentPlot->axisRect()->setupFullAxesBox();
    ui->momentPlot->rescaleAxes();
    ui->momentPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
void MainWindow::on_analyzeButton_clicked()
{
    doAnalysis();
}
*/

/* ***** menu actions ***** */

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

/* ***** check box status changes ***** */

void MainWindow::on_chkBox_assume_rigid_cap_clicked(bool checked)
{
    assumeRigidPileHead = checked;

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
    P = ui->appliedForce->value();

    int sliderPosition = 10*int(P/3500.0);
    if (sliderPosition >  10) sliderPosition= 10;
    if (sliderPosition < -10) sliderPosition=-10;
    ui->displacementSlider->setValue(sliderPosition);

    this->doAnalysis();
}

void MainWindow::on_appliedForce_editingFinished()
{
    P = ui->appliedForce->value();

    int sliderPosition = 10*int(P/3500.0);
    if (sliderPosition >  10) sliderPosition= 10;
    if (sliderPosition < -10) sliderPosition=-10;
    ui->displacementSlider->setValue(sliderPosition);

    this->doAnalysis();
}

void MainWindow::on_displacementSlider_sliderMoved(int position)
{
    // slider moved -- the number of steps (10) is a parameter to the slider in mainwindow.ui
    displacementRatio = double(position)/10.0;

    P = 3500.0 * displacementRatio;
    ui->appliedForce->setValue(P);

    this->doAnalysis();
}

void MainWindow::on_displacementSlider_actionTriggered(int action)
{
    // slider moved -- the number of steps (10) is a parameter to the slider in mainwindow.ui
    int position = ui->displacementSlider->value();
    displacementRatio = double(position)/10.0;

    P = 3500.0 * displacementRatio;
    ui->appliedForce->setValue(P);

    this->doAnalysis();
}

/* ***** analysis parameter changes ***** */

void MainWindow::on_pileDiameter_valueChanged(double arg1)
{
    pileDiameter = arg1;
    this->doAnalysis();
}

void MainWindow::on_embeddedLength_valueChanged(double arg1)
{
    L2 = arg1;
    this->doAnalysis();
}

void MainWindow::on_freeLength_valueChanged(double arg1)
{
    L1 = arg1;
    this->doAnalysis();
}

void MainWindow::on_Emodulus_valueChanged(double arg1)
{
    E = arg1;
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
 if (item && item == ui->matTable->currentItem()) {
      if(item->row() == 0)
         mSoilLayers[item->column()].setLayerThickness(item->text().toDouble());
      else if (item->row() == 1)
         mSoilLayers[item->column()].setLayerUnitWeight(item->text().toDouble());
      else if (item->row() == 2)
         mSoilLayers[item->column()].setLayerSatUnitWeight(item->text().toDouble());
      else if (item->row() == 3)
         mSoilLayers[item->column()].setLayerFrictionAng(item->text().toDouble());
      else if (item->row() == 4)
         mSoilLayers[item->column()].setLayerStiffness(item->text().toDouble());
  }
}
