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

    // setup table of soil layers
    QStringList matTableLabels;
    matTableLabels << "Thickness" << "Unit Weight" << "Friction Angle" << "Shear Modulus";
    ui->matTable->setRowCount(4);
    ui->matTable->setColumnCount(1);
    ui->matTable->setVerticalHeaderLabels(matTableLabels);
    ui->matTable->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
    ui->matTable->setItemPrototype(ui->matTable->item(3,0));
    ui->matTable->setItemDelegate(new QItemDelegate());
    connect(ui->matTable, SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)),
          this, SLOT(updateInfo(QTableWidgetItem*)));
    connect(ui->matTable, SIGNAL(itemChanged(QTableWidgetItem*)),
          this, SLOT(updateInfo(QTableWidgetItem*)));
    setupLayers();
    reDrawTable();    

    // setup data
    P        = 0.0;
    L1       = 1.0;
    L2       = 20.0;
    D        = 1.0;
    gwtDepth = 3.00;
    E        = 25.0e6;
    numEle   = 80;
    gamma    = 17.0;
    phi      = 36.0;
    gSoil    = 150000;
    puSwitch = 1;
    kSwitch  = 1;
    gwtSwitch= 1;

    ui->appliedForce->setValue(P);
    ui->pileDiameter->setValue(D);
    ui->freeLength->setValue(L1);
    ui->embeddedLength->setValue(L2);
    ui->Emodulus->setValue(E);
    ui->groundWaterTable->setValue(gwtDepth);

    // set initial state of check boxes
    useToeResistance    = true;
    assumeRigidPileHead = true;

    ui->chkBox_assume_rigid_cap->setCheckState(assumeRigidPileHead?Qt::Checked:Qt::Unchecked);
    ui->chkBox_include_toe_resistance->setCheckState(useToeResistance?Qt::Checked:Qt::Unchecked);

    // analysis parameters
    displacementRatio = 0.0;

    this->doAnalysis();

}
void
MainWindow::doAnalysis(void)
{
  // clear existing model
  theDomain.clearAll();
  OPS_clearAllUniaxialMaterial();
  ops_Dt = 0.0;

  double eleSize = (L1+L2)/(1.0*numEle);
  int numNodePile = 1+numEle;

  //
  // create spring nodes
  //

  int numNodeEmbedded;
  for (int i=1; i<=numNodePile; i++) {
    double zCoord = eleSize*(i-1);
    if (zCoord <= L2) {
        Node *theNode = 0;
        theNode = new Node(    i, 3, 0., 0., zCoord);  theDomain.addNode(theNode);
        theNode = new Node(i+100, 3, 0., 0., zCoord);  theDomain.addNode(theNode);

        SP_Constraint *theSP = 0;
        theSP = new SP_Constraint(i, 0, 0., true);  theDomain.addSP_Constraint(theSP);
        theSP = new SP_Constraint(i, 1, 0., true);  theDomain.addSP_Constraint(theSP);
        theSP = new SP_Constraint(i, 2, 0., true);  theDomain.addSP_Constraint(theSP);
        theSP = new SP_Constraint(i+100, 1, 0., true);  theDomain.addSP_Constraint(theSP);
        theSP = new SP_Constraint(i+100, 2, 0., true);  theDomain.addSP_Constraint(theSP);

        numNodeEmbedded=i;
    }
  }

  //
  // create pile node
  //

  for (int i=1; i<=numNodePile; i++) {
    double zCoord = eleSize*(i-1);
    Node *theNode = 0;
    int nodeTag = i+200;
    theNode = new Node(nodeTag, 6, 0., 0., zCoord);  theDomain.addNode(theNode);
    if (i == numNodePile && assumeRigidPileHead ) {
      SP_Constraint *theSP = 0;
      theSP = new SP_Constraint(nodeTag, 4, 0., true); theDomain.addSP_Constraint(theSP);
    }
    if (i != 1) {
      SP_Constraint *theSP = 0;
      theSP = new SP_Constraint(nodeTag, 1, 0., true); theDomain.addSP_Constraint(theSP);
      theSP = new SP_Constraint(nodeTag, 3, 0., true); theDomain.addSP_Constraint(theSP);
      theSP = new SP_Constraint(nodeTag, 5, 0., true); theDomain.addSP_Constraint(theSP);
}
  }

  //
  // constrain spring and pile nodes with equalDOF (identity constraints)
  //
  static Matrix Ccr (2, 2);
  Ccr.Zero(); Ccr(0,0)=1.0; Ccr(1,1)=1.0;
  static ID rcDof (2);
  rcDof(0) = 0;
  rcDof(1) = 2;
  for (int i=1; i<=numNodeEmbedded; i++)  {
    MP_Constraint *theMP = new MP_Constraint(i+200, i+100, Ccr, rcDof, rcDof);
    theDomain.addMP_Constraint(theMP);
  }

  //
  // create soil-spring materials
  //

  // # p-y spring material
  for (int i=1; i <= numNodeEmbedded; i++) {
    double pyDepth = L2 - eleSize*(i - 1);
    double pult, y50;
    getPyParam(pyDepth, gamma, phi, D, eleSize, puSwitch, kSwitch, gwtSwitch, &pult, &y50);
    UniaxialMaterial *theMat = new PySimple1(i, 0, 2, pult, y50, 0.0, 0.0);
    OPS_addUniaxialMaterial(theMat);
  }


  // t-z spring material
  for (int i=2; i <= numNodeEmbedded; i++) {
    double pyDepth = eleSize*(i - 1);
    double sigV = gamma*pyDepth;
    double tult, z50;
    getTzParam(phi, D,  sigV,  eleSize, &tult, &z50);
    UniaxialMaterial *theMat = new TzSimple1(i+100, 0, 2, tult, z50, 0.0);
    OPS_addUniaxialMaterial(theMat);
  }

  // # q-z spring material
  // # vertical effective stress at pile tip, no water table (depth is embedded pile length)
  double sigVq  = gamma*L2;
  double qult, z50q;
  getQzParam(phi, D,  sigVq,  gSoil, &qult, &z50q);
  UniaxialMaterial *theMat = new QzSimple1(1+100, 2, qult, z50q, 0.0, 0.0);
  OPS_addUniaxialMaterial(theMat);


  //
  // create soil spring elements
  //

  // create the vectors for the element orientation
  static Vector x(3); x(0) = 1.0; x(1) = 0.0; x(2) = 0.0;
  static Vector y(3); y(0) = 0.0; y(1) = 1.0; y(2) = 0.0;

  UniaxialMaterial *theMaterials[2];
  theMaterials[0] = OPS_getUniaxialMaterial(1);
  theMaterials[1] = OPS_getUniaxialMaterial(1+100);
  ID direction(2);
  direction(0) = 0;
  direction[1] = 2;
  Element *theEle = new ZeroLength(10001, 3, 1, 1+100, x, y, 2, theMaterials, direction);
  theDomain.addElement(theEle);


  for (int i=2; i<=numNodeEmbedded; i++) {
      theMaterials[0] = OPS_getUniaxialMaterial(i);
      theMaterials[1] = OPS_getUniaxialMaterial(i+100);
      Element *theEle = new ZeroLength(i+1000, 3, i, i+100, x, y, 2, theMaterials, direction);
      theDomain.addElement(theEle);
  }

  //
  // create pile elements
  //

  static Vector crdV(3); crdV(0)=0.; crdV(1)=-1; crdV(2) = 0.;
  CrdTransf *theTransformation = new LinearCrdTransf3d(1, crdV);

  double  PI= 3.14159;
  double A  = PI * D * D/4.0;
  double Iz = A * D * D/16.0;
  double G = E/(2.0*(1+0.3));
  double J = 1.0e10;

 for (int i=1; i<=numEle; i++) {
     BeamIntegration *theIntegration = new LegendreBeamIntegration();
     SectionForceDeformation *theSections[3];
     SectionForceDeformation *theSection = new ElasticSection3d(1, E, A, Iz, Iz, G, J);
     theSections[0] = theSection;
     theSections[1] = theSection;
     theSections[2] = theSection;
     Element *theEle = new DispBeamColumn3d(i+200, i+200, i+201, 3, theSections, *theIntegration, *theTransformation);
     theDomain.addElement(theEle);
     delete theSection;
     delete theIntegration;
  }

 //
 // create load pattern and add loads
 //

 LinearSeries *theTimeSeries = new LinearSeries(1, 1.0);
 LoadPattern *theLoadPattern = new LoadPattern(1);
 theLoadPattern->setTimeSeries(theTimeSeries);
 static Vector load(6); load.Zero(); load(0) = P;
 NodalLoad *theLoad = new NodalLoad(0, numNodePile+200, load);
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


 double maxDisp = 0;
 double minDisp = 0;
 double maxMoment = 0;
 double minMoment = 0;

 for (int i=1; i<=numNodePile; i++) {
     zero[i-1] = 0.0;
     Node *theNode = theDomain.getNode(i+200);
     const Vector &nodeCoord = theNode->getCrds();
     loc[i-1] = nodeCoord(2)-L2;
     const Vector &nodeDisp = theNode->getDisp();
     disp[i-1] = nodeDisp(0);
     if (disp[i-1] > maxDisp) maxDisp = disp[i-1];
     if (disp[i-1] < minDisp) minDisp = disp[i-1];
 }


 for (int i=1; i<=numEle; i++) {
     Element *theEle = theDomain.getElement(i+200);
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

// ui->momentPlot->clearGraphs();
// ui->momentPlot->addGraph();
// ui->momentPlot->graph(0)->setData(zero,loc);
// ui->momentPlot->graph(0)->setPen(QPen(Qt::black));
// ui->momentPlot->addGraph();
// ui->momentPlot->graph(1)->setData(moment,loc);
// ui->momentPlot->graph(1)->setPen(QPen(Qt::blue));
// ui->momentPlot->xAxis->setRange(minMoment,maxMoment);
// ui->momentPlot->yAxis->setRange(-L2,L1);
// ui->momentPlot->replot();

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
}

void MainWindow::on_chkBox_include_toe_resistance_clicked(bool checked)
{
    useToeResistance = checked;
}

/* ***** analysis parameter changes ***** */

void MainWindow::on_displacementSlider_sliderMoved(int position)
{
    // slider moved -- the number of steps (10) is a parameter to the slider in mainwindow.ui
    displacementRatio = double(position)/10.0;

    P = 3500.0 * displacementRatio;
    ui->appliedForce->setValue(P);
    this->doAnalysis();
}

void MainWindow::on_pileDiameter_valueChanged(double arg1)
{
    D = arg1;
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


void MainWindow::setupLayers()
{
    soilLayers.clear();
    soilLayers.push_back(soilLayer("Layer 1", 3.0, 18.0, 2.0e5, 30, QColor(100,0,0,100)));
    soilLayers.push_back(soilLayer("Layer 2", 3.0, 19.0, 2.0e5, 35, QColor(0,100,0,100)));
    soilLayers.push_back(soilLayer("Layer 3", 4.0, 17.0, 2.0e5, 25, QColor(0,0,100,100)));
}

void MainWindow::reDrawTable()
{
  // update and redraw the ui->matTable
    int numLayers = soilLayers.size();
    QStringList matTableHeaders;
    ui->matTable->setColumnCount(numLayers);
    for (int ii = 0; ii < numLayers; ii++)
    {
        matTableHeaders << soilLayers[ii].getLayerName();
        ui->matTable->setItem(0, ii, new QTableWidgetItem(QString::number(soilLayers[ii].getLayerThickness())));
        ui->matTable->setItem(1, ii, new QTableWidgetItem(QString::number(soilLayers[ii].getLayerUnitWeight())));
        ui->matTable->setItem(2, ii, new QTableWidgetItem(QString::number(soilLayers[ii].getLayerFrictionAng())));
        ui->matTable->setItem(3, ii, new QTableWidgetItem(QString::number(soilLayers[ii].getLayerStiffness())));
    }

    ui->matTable->setHorizontalHeaderLabels(matTableHeaders);

    for (int ii = 0; ii < ui->matTable->rowCount(); ii++)
	for (int jj = 1; jj < ui->matTable->columnCount(); jj++)
		ui->matTable->item(ii,jj)->setTextAlignment(Qt::AlignHCenter);

}

void MainWindow::updateInfo(QTableWidgetItem * item)
{
 if (item && item == ui->matTable->currentItem()) {
      if(item->row() == 0)
         soilLayers[item->column()].setLayerThickness(item->text().toDouble());
      else if (item->row() == 1)
         soilLayers[item->column()].setLayerUnitWeight(item->text().toDouble());
      else if (item->row() == 2)
         soilLayers[item->column()].setLayerFrictionAng(item->text().toDouble());
      else if (item->row() == 3)
         soilLayers[item->column()].setLayerStiffness(item->text().toDouble());
  }
  return;
}


void MainWindow::on_appliedForce_valueChanged(double arg1)
{
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
