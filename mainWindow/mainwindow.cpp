#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include "stdlib.h"
#include "utilWindows/copyrightdialog.h"
#include "utilWindows/dialogpreferences.h"
#include "utilWindows/dialogabout.h"
#include "utilWindows/dialogquicktips.h"
#include "utilWindows/dialogfuturefeature.h"
#include "pilefeamodeler.h"

#include "systemplotsuper.h"
#include "systemplotqcp.h"
#include "systemplotqwt.h"

#include "resultplotsuper.h"
#include "resultplotqcp.h"
#include "resultplotqwt.h"

#include <QApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QDateTime>
#include <QFileDialog>
#include <QDir>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

// OpenSees include files
#include <StandardStream.h>

StandardStream sserr;
OPS_Stream *opserrPtr = &sserr;

//SimulationInformation simulationInfo;

MainWindow::MainWindow(bool graphicsModeQCP, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    inSetupState = true;

    ui->setupUi(this);
    ui->tabWidget->tabBar()->setUsesScrollButtons(true);

    this->fetchSettings();

#ifdef USEQCP
    if (graphicsModeQCP || useGraphicsLib == "QCP")
    {
        systemPlot  = new SystemPlotQCP(ui->systemTab);
        displPlot   = new ResultPlotQCP(ui->dispTab);
        pullOutPlot = new ResultPlotQCP(ui->pulloutTab);
        momentPlot  = new ResultPlotQCP(ui->momentTab);
        shearPlot   = new ResultPlotQCP(ui->shearTab);
        axialPlot   = new ResultPlotQCP(ui->axialTab);
        stressPlot  = new ResultPlotQCP(ui->stressTab);
        pultPlot    = new ResultPlotQCP(ui->pultTab);
        y50Plot     = new ResultPlotQCP(ui->y50Tab);
        tultPlot    = new ResultPlotQCP(ui->tultTab);
        z50Plot     = new ResultPlotQCP(ui->z50Tab);
    }
    else
#endif
    {
        systemPlot  = new SystemPlotQwt(ui->systemTab);
        displPlot   = new ResultPlotQwt(ui->dispTab);
        pullOutPlot = new ResultPlotQwt(ui->pulloutTab);
        momentPlot  = new ResultPlotQwt(ui->momentTab);
        shearPlot   = new ResultPlotQwt(ui->shearTab);
        axialPlot   = new ResultPlotQwt(ui-> axialTab);
        stressPlot  = new ResultPlotQwt(ui->stressTab);
        pultPlot    = new ResultPlotQwt(ui->pultTab);
        y50Plot     = new ResultPlotQwt(ui->y50Tab);
        tultPlot    = new ResultPlotQwt(ui->tultTab);
        z50Plot     = new ResultPlotQwt(ui->z50Tab);
    }

    //
    // place widgets into their respective layouts
    //
    QLayout *lyt;

    lyt = ui->systemTab->layout();  lyt->addWidget(systemPlot);
    lyt = ui->dispTab->layout();    lyt->addWidget(displPlot);
    lyt = ui->pulloutTab->layout(); lyt->addWidget(pullOutPlot);
    lyt = ui->momentTab->layout();  lyt->addWidget(momentPlot);
    lyt = ui->shearTab->layout();   lyt->addWidget(shearPlot);
    lyt = ui->axialTab->layout();   lyt->addWidget(axialPlot);
    lyt = ui->stressTab->layout();  lyt->addWidget(stressPlot);
    lyt = ui->pultTab->layout();    lyt->addWidget(pultPlot);
    lyt = ui->y50Tab->layout();     lyt->addWidget(y50Plot);
    lyt = ui->tultTab->layout();    lyt->addWidget(tultPlot);
    lyt = ui->z50Tab->layout();     lyt->addWidget(z50Plot);

    ui->tabWidget->setCurrentWidget(ui->dispTab);

    //
    // general setup
    //
    this->updateUI();
    ui->headerWidget->setHeadingText("SimCenter Pile Group Tool");

    ui->HForceCtrl->setRange(-MAX_H_FORCE, MAX_H_FORCE);
    ui->VForceCtrl->setRange(-MAX_V_FORCE, MAX_V_FORCE);
    ui->MomentCtrl->setRange(-MAX_MOMENT, MAX_MOMENT);

    ui->PushOverCtrl->setRange(-MAX_DISP*1000,MAX_DISP*1000);  // display unit is mm
    ui->PullOutCtrl->setRange(-MAX_DISP*1000,MAX_DISP*1000);   // display unit is mm

    ui->SurfaceDisplacementCtrl->setRange(-MAX_DISP*1000,MAX_DISP*1000);  // display unit is mm
    ui->Interface12Ctrl->setRange(-100,100);                              // display units is %
    ui->Interface12Ctrl->setRange(-100,100);                              // display units is %
    ui->BaseDisplacementCtrl->setRange(-100,100);                         // display units is %

    /* connect a FEA modeler */
    pileFEAmodel = new PileFEAmodeler();
    loadControlType = LoadControlType::ForceControl;
    pileFEAmodel->setLoadType(loadControlType);

    // setup data
    numPiles = 1;
    P        = MAX_H_FORCE/10.0;
    PV       = MAX_V_FORCE/10.0;
    PMom     = 0.0;

    HDisp = 0.10; // prescribed horizontal displacement
    VDisp = 0.01; // prescriber vertical displacement

    surfaceDisp    = 0.1; // prescribed soil surface displacement
    percentage12   = 1.0; // percentage of surface displacement at 1st layer interface
    percentage23   = 0.0; // percentage of surface displacement at 2nd layer interface
    percentageBase = 0.0; // percentage of surface displacement at base of soil column

    L1                       = 1.0;
    L2[numPiles-1]           = 20.0;
    pileDiameter[numPiles-1] = 1.0;
    E[numPiles-1]            = 25.0e6;
    xOffset[numPiles-1]      = 0.0;

    gwtDepth = 4.00;
    gSoil    = 150000;
    puSwitch = 2;
    kSwitch  = 1;
    gwtSwitch= 1;

     // set initial state of check boxes
    useToeResistance              = true;
    assumeRigidPileHeadConnection = false;

    // set up initial values before activating live analysis (= connecting the slots)
    //    or the program will fail in the analysis due to missing information
    setupLayers();

    //reDrawTable();

    setActiveLayer(0);

    // set up pile input
    this->refreshUI();

    // plot setting
    activePileIdx = 0;
    activeLayerIdx = -1;

    //
    // this connect statement needs to be updated to reflect changes to the SystemPlotSuper class
    //
    QObject::connect(systemPlot, SIGNAL(on_pileSelected(int)), this, SLOT(onSystemPlot_pileSelected(int)));
    QObject::connect(systemPlot, SIGNAL(on_soilLayerSelected(int)), this, SLOT(onSystemPlot_soilLayerSelected(int)));
    QObject::connect(systemPlot, SIGNAL(on_groundWaterSelected()), this, SLOT(onSystemPlot_groundWaterSelected()));

    inSetupState = false;

    //
    // adjust size of application window to the available display
    //
    QRect rec = QApplication::desktop()->screenGeometry();
    int height = this->height()<0.85*rec.height()?0.85*rec.height():this->height();
    int width  = this->width()<0.85*rec.width()?0.85*rec.width():this->width();
    this->resize(width, height);

    //
    // create the initial set of plots
    //
    this->doAnalysis();
    this->updateSystemPlot();

    //
    // connect to the application usage counter
    //
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://opensees.berkeley.edu/OpenSees/developer/qtPile/use.php")));
    //manager->get(QNetworkRequest(QUrl("https://simcenter.designsafe-ci.org/pile-group-analytics/")));

    this->on_forceTypeSelector_activated(0);
}

MainWindow::~MainWindow()
{
    delete ui;

    if (pileFEAmodel != nullptr) delete pileFEAmodel;
}

void MainWindow::refreshUI() {

    inSetupState = true;

    ui->pileIndex->setValue(1);
    ui->pileIndex->setMinimum(1);
    ui->pileIndex->setMaximum(numPiles);

    if (loadControlType == LoadControlType::ForceControl) { ui->forceTypeSelector->setCurrentIndex(0); }
    else if (loadControlType == LoadControlType::PushOver) { ui->forceTypeSelector->setCurrentIndex(1); }
    else if (loadControlType == LoadControlType::SoilMotion) { ui->forceTypeSelector->setCurrentIndex(2); }
    else { ui->forceTypeSelector->setCurrentIndex(0); }

    ui->HForceCtrl->setValue(std::nearbyint(P));
    ui->VForceCtrl->setValue(std::nearbyint(PV));
    ui->MomentCtrl->setValue(std::nearbyint(PMom));

    // TODO
    //this->on_appliedHorizontalForce_editingFinished();
    //this->on_appliedVerticalForce_editingFinished();
    //this->on_appliedMoment_editingFinished();

    int pileIdx = ui->pileIndex->value() - 1;

    ui->xOffset->setValue(xOffset[pileIdx]);
    ui->pileDiameter->setValue(pileDiameter[pileIdx]);
    ui->freeLength->setValue(L1);
    ui->embeddedLength->setValue(L2[pileIdx]);
    ui->Emodulus->setValue( (E[pileIdx]/1.0e+6) );

    ui->groundWaterTable->setValue(gwtDepth);

    ui->chkBox_assume_rigid_cap->setCheckState(assumeRigidPileHeadConnection?Qt::Checked:Qt::Unchecked);
    ui->chkBox_include_toe_resistance->setCheckState(useToeResistance?Qt::Checked:Qt::Unchecked);

    this->updateLayerState();

    inSetupState = false;
}

void MainWindow::doAnalysis(void)
{
    if (inSetupState) return;

    //
    // create pile information for plotting
    //
    QVector<PILE_INFO> pileInfo;

    pileInfo.clear();
    for (int i=0; i<numPiles; i++)
    {
        PILE_INFO thisPile;
        thisPile.L1           = L1;
        thisPile.L2           = L2[i];
        thisPile.pileDiameter = pileDiameter[i];
        thisPile.xOffset      = xOffset[i];
        thisPile.E            = E[i];
        pileInfo.append(thisPile);
    }
    pileFEAmodel->updatePiles(pileInfo);

    //
    // switches
    //
    pileFEAmodel->updateSwitches(useToeResistance, assumeRigidPileHeadConnection);

    //
    // set the soil layer information
    //
    pileFEAmodel->updateSoil(mSoilLayers);

    //
    // update ground water table
    //
    pileFEAmodel->updateGWtable(gwtDepth);

    //
    // set the load control type
    //
    pileFEAmodel->setLoadType(loadControlType);

    //
    // update the load control data
    //
    switch (loadControlType) {
    case LoadControlType::ForceControl:
        pileFEAmodel->updateLoad(P, PV, PMom);
        break;
    case LoadControlType::PushOver:
        pileFEAmodel->updateDisplacement(HDisp, VDisp);
        break;
    case LoadControlType::SoilMotion:
        QVector<double> profile;
        profile.append(surfaceDisp);
        profile.append(percentage12);
        profile.append(percentage23);
        profile.append(percentageBase);
        pileFEAmodel->updateDispProfile(profile);
        break;
    }

    //
    // run the analysis
    //
    bool converged = pileFEAmodel->doAnalysis();
    systemPlot->setSystemStable(converged);

    //
    // plot results
    //
    this->updateResultPlots();
}

void MainWindow::updateResultPlots()
{
    // this should call the results update from the analysis modeler and the plot methods

    //
    // send deformations to systemPlot
    //

    QList<QVector<QVector<double> *> *> list;
    list = pileFEAmodel->getLateralDisplacements();
    if (list.size() >= 2)
    {
        QVector<QVector<double> *> &pos   = *list[0];
        QVector<QVector<double> *> &dispU = *list[1];
        list = pileFEAmodel->getAxialDisplacements();
        QVector<QVector<double> *> &dispV = *list[1];

        this->systemPlot->updatePileDeformation(pos, dispU, dispV);
    }
    else
    {
        QVector<QVector<double> *> dummy;
        this->systemPlot->updatePileDeformation(dummy, dummy, dummy);
    }


    //
    // plot results
    //

    // lateral displacements
    if (showDisplacements) {
        QList<QVector<QVector<double> *> *> list = pileFEAmodel->getLateralDisplacements();
        if (list.size() >= 2)
            displPlot->plotResults(*list[0], *list[1], "Depth [m]", "Lateral Displacement [m]");
    }

    // axial displacements
    if (showPullOut) {
        QList<QVector<QVector<double> *> *> list = pileFEAmodel->getAxialDisplacements();
        if (list.size() >= 2)
            pullOutPlot->plotResults(*list[0], *list[1], "Depth [m]", "Axial Displacement [m]");
    }

    // axial
    if (showAxial) {
        QList<QVector<QVector<double> *> *> list = pileFEAmodel->getForce();
        if (list.size() >= 2)
            axialPlot->plotResults(*list[0], *list[1], "Depth [m]", "Axial Force [kN]");
    }

    // shear
    if (showShear) {
        QList<QVector<QVector<double> *> *> list = pileFEAmodel->getShear();
        if (list.size() >= 2)
            shearPlot->plotResults(*list[0], *list[1], "Depth [m]", "Shear Force [kN]");
    }

    // moments
    if (showMoments) {
        QList<QVector<QVector<double> *> *> list = pileFEAmodel->getMoment();
        if (list.size() >= 2)
            momentPlot->plotResults(*list[0], *list[1], "Depth [m]", "Pile Moment [kN.m]");
    }

    // vertical stress
    if (showStress) {
        QList<QVector<QVector<double> *> *> list = pileFEAmodel->getStress();
        if (list.size() >= 2)
            stressPlot->plotResults(*list[0], *list[1], "Depth [m]", "Effective Stress [kPa]");
    }

    // p_ultimate
    if (showPultimate) {
        QList<QVector<QVector<double> *> *> list = pileFEAmodel->getPult();
        if (list.size() >= 2)
            pultPlot->plotResults(*list[0], *list[1], "Depth [m]", "p_ult [kPa]");
    }

    // y_50
    if (showY50) {
        QList<QVector<QVector<double> *> *> list = pileFEAmodel->getY50();
        if (list.size() >= 2)
            y50Plot->plotResults(*list[0], *list[1], "Depth [m]", "y_50 [m]");
    }

    // t_ultimate
    if (showTultimate) {
        QList<QVector<QVector<double> *> *> list = pileFEAmodel->getTult();
        if (list.size() >= 2)
            tultPlot->plotResults(*list[0], *list[1], "Depth [m]", "t_ult [kPa]");
    }

    // z_50
    if (showZ50) {
        QList<QVector<QVector<double> *> *> list = pileFEAmodel->getZ50();
        if (list.size() >= 2)
            z50Plot->plotResults(*list[0], *list[1], "Depth [m]", "z_50 [m]");
    }
}

void MainWindow::fetchSettings()
{
    if (settings != nullptr) { delete settings; }
    settings = new QSettings("NHERI SimCenter","Pile Group Tool");

    // general settings
    settings->beginGroup("general");
        useGraphicsLib    = settings->value("graphicsLibrary", QString("QCP")).toString();
        if (useGraphicsLib == "Qwt") { useGraphicsLib = "QwtAll"; }
        useFEAnalyzer     = settings->value("femAnalyzer", QString("OpenSeesInt")).toString();
    settings->endGroup();

    // viewer settings
    settings->beginGroup("viewer");
        showDisplacements = settings->value("displacements",1).toBool();
        showPullOut       = settings->value("pullout",1).toBool();
        showMoments       = settings->value("moments",1).toBool();
        showShear         = settings->value("shear",1).toBool();
        showAxial         = settings->value("axial",1).toBool();
        showStress        = settings->value("stress",1).toBool();
        showPultimate     = settings->value("pult",1).toBool();
        showY50           = settings->value("compliance",1).toBool();
        showTultimate     = settings->value("tult",1).toBool();
        showZ50           = settings->value("Zcompliance",1).toBool();
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

//
// UI helper functions
//

void MainWindow::setupLayers()
{
    mSoilLayers.clear();
    mSoilLayers.push_back(soilLayer("Layer 1", 3.0, 15.0, 18.0, 2.0e5, 30, 0.0, QColor(100,0,0,100)));
    mSoilLayers.push_back(soilLayer("Layer 2", 3.0, 16.0, 19.0, 2.0e5, 35, 0.0, QColor(0,100,0,100)));
    mSoilLayers.push_back(soilLayer("Layer 3",14.0, 14.0, 17.0, 2.0e5, 28, 0.0, QColor(0,0,100,100)));

    updateLayerState();
}

void MainWindow::updateLayerState()
{
    // set depth and GWT depth for each layer
    int numLayers = mSoilLayers.size();

    double layerDepthFromSurface = 0.0;
    for (int ii = 0; ii < numLayers; ii++)
    {
        mSoilLayers[ii].setLayerDepth(layerDepthFromSurface);
        mSoilLayers[ii].setGWTdepth(gwtDepth - layerDepthFromSurface);
        layerDepthFromSurface += mSoilLayers[ii].getLayerThickness();
        if (ii > 0)
            mSoilLayers[ii].setLayerTopStress(mSoilLayers[ii-1].getLayerBottomStress());
    }
}

void MainWindow::updateUI()
{
    if (!showDisplacements && ui->tabWidget->indexOf(ui->dispTab)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->dispTab));
    }
    if (!showPullOut && ui->tabWidget->indexOf(ui->pulloutTab)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->pulloutTab));
    }
    if (!showMoments && ui->tabWidget->indexOf(ui->momentTab)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->momentTab));
    }
    if (!showShear && ui->tabWidget->indexOf(ui->shearTab)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->shearTab));
    }
    if (!showAxial && ui->tabWidget->indexOf(ui->axialTab)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->axialTab));
    }
    if (!showStress && ui->tabWidget->indexOf(ui->stressTab)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->stressTab));
    }
    if (!showPultimate && ui->tabWidget->indexOf(ui->pultTab)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->pultTab));
    }
    if (!showY50 && ui->tabWidget->indexOf(ui->y50Tab)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->y50Tab));
    }
    if (!showTultimate && ui->tabWidget->indexOf(ui->tultTab)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tultTab));
    }
    if (!showZ50 && ui->tabWidget->indexOf(ui->z50Tab)>=0 ) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->z50Tab));
    }

    int numTabs = ui->tabWidget->count();

    if (showDisplacements && ui->tabWidget->indexOf(ui->dispTab) < 0 ) {
        ui->tabWidget->addTab(ui->dispTab,"Lateral Disp");
    }
    if (showPullOut && ui->tabWidget->indexOf(ui->pulloutTab) < 0 ) {
        ui->tabWidget->addTab(ui->pulloutTab,"Axial Disp");
    }
    if (showMoments && ui->tabWidget->indexOf(ui->momentTab) < 0 ) {
        ui->tabWidget->addTab(ui->momentTab,"Moment");
    }
    if (showShear && ui->tabWidget->indexOf(ui->shearTab) < 0 ) {
        ui->tabWidget->addTab(ui->shearTab,"Shear");
    }
    if (showAxial && ui->tabWidget->indexOf(ui->axialTab) < 0 ) {
        ui->tabWidget->addTab(ui->axialTab,"Axial");
    }
    if (showStress && ui->tabWidget->indexOf(ui->stressTab) < 0 ) {
        ui->tabWidget->addTab(ui->stressTab,"Stress");
    }
    if (showPultimate && ui->tabWidget->indexOf(ui->pultTab) < 0 ) {
        ui->tabWidget->addTab(ui->pultTab,"p_ult");
    }
    if (showY50 && ui->tabWidget->indexOf(ui->y50Tab) < 0) {
        ui->tabWidget->addTab(ui->y50Tab,"y50");
    }
    if (showTultimate && ui->tabWidget->indexOf(ui->tultTab) < 0 ) {
        ui->tabWidget->addTab(ui->tultTab,"t_ult");
    }
    if (showZ50 && ui->tabWidget->indexOf(ui->z50Tab) < 0) {
        ui->tabWidget->addTab(ui->z50Tab,"z50");
    }
}

//
// menu actions
//

void MainWindow::on_actionNew_triggered()
{
    this->on_actionReset_triggered();
}

void MainWindow::on_action_Open_triggered()
{
    if ( this->ReadFile("PileTool.json") ) {
        this->refreshUI();
        this->updateSystemPlot();
        this->doAnalysis();
    }
}

void MainWindow::on_actionSave_triggered()
{
    this->WriteFile("PileTool.json");
}

void MainWindow::on_actionExport_to_OpenSees_triggered()
{
    QString theFolder = QDir::homePath();
    QString theFilter = "Tcl file (*.tcl)";
    QFileDialog dlg;

    // "PileGroup.tcl"

    QString fileName = dlg.getSaveFileName(this, "Save File", theFolder, theFilter);

    if (fileName != "")
    {
        //pileFEAmodel->dumpDomain("domainDump.tcl");
        pileFEAmodel->writeFEMinput(fileName);
    }
}

void MainWindow::on_actionReset_triggered()
{
    inSetupState = true;

    this->fetchSettings();
    this->updateUI();

    // setup data
    numPiles = 1;
    P        = MAX_H_FORCE/10.0;
    PV       = MAX_V_FORCE/10.0;
    PMom     = 0.0;

    HDisp = 0.10; // prescribed horizontal displacement
    VDisp = 0.01; // prescriber vertical displacement

    surfaceDisp    = 0.1; // prescribed soil surface displacement
    percentage12   = 1.0; // percentage of surface displacement at 1st layer interface
    percentage23   = 0.0; // percentage of surface displacement at 2nd layer interface
    percentageBase = 0.0; // percentage of surface displacement at base of soil column

    L1                       = 1.0;
    L2[numPiles-1]           = 20.0;
    pileDiameter[numPiles-1] = 1.0;
    E[numPiles-1]            = 25.0e6;
    xOffset[numPiles-1]      = 0.0;

    gwtDepth = 4.00;
    gSoil    = 150000;
    puSwitch = 2;
    kSwitch  = 1;
    gwtSwitch= 1;

     // set initial state of check boxes
    useToeResistance              = true;
    assumeRigidPileHeadConnection = false;

    // meshing parameters
    minElementsPerLayer = MIN_ELEMENTS_PER_LAYER;
    maxElementsPerLayer = MAX_ELEMENTS_PER_LAYER;
    numElementsInAir    = NUM_ELEMENTS_IN_AIR;

    // set up initial values before activating live analysis (= connecting the slots)
    //    or the program will fail in the analysis due to missing information
    setupLayers();

    //reDrawTable();

    setActiveLayer(0);

    // set up pile input

    ui->pileIndex->setValue(1);
    ui->pileIndex->setMinimum(1);
    ui->pileIndex->setMaximum(numPiles);

    int pileIdx = ui->pileIndex->value() - 1;

    ui->HForceCtrl->setValue(std::nearbyint(P));
    ui->xOffset->setValue(xOffset[pileIdx]);
    ui->pileDiameter->setValue(pileDiameter[pileIdx]);
    ui->freeLength->setValue(L1);
    ui->embeddedLength->setValue(L2[pileIdx]);
    ui->Emodulus->setValue( (E[pileIdx]/1.0e+6) );

    ui->groundWaterTable->setValue(gwtDepth);

    ui->chkBox_assume_rigid_cap->setCheckState(assumeRigidPileHeadConnection?Qt::Checked:Qt::Unchecked);
    ui->chkBox_include_toe_resistance->setCheckState(useToeResistance?Qt::Checked:Qt::Unchecked);

    // plotsetting
    activePileIdx = 0;
    activeLayerIdx = -1;

    inSetupState = false;

    this->doAnalysis();
    this->updateSystemPlot();
}

void MainWindow::on_actionFEA_parameters_triggered()
{
    this->on_actionPreferences_triggered();
}


void MainWindow::on_action_About_triggered()
{
    DialogAbout *dlg = new DialogAbout();

    //
    // adjust size of application window to the available display
    //
    QRect rec = QApplication::desktop()->screenGeometry();
    int height = 0.50*rec.height();
    int width  = 0.50*rec.width();
    dlg->resize(width, height);

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

//
// SLOTS (call-back functions)
//

void MainWindow::on_btn_deletePile_clicked()
{
    int pileIdx = ui->pileIndex->value() - 1;
    if (pileIdx < numPiles && numPiles > 1) {
        if ( pileIdx < numPiles ) {
            for (int j=pileIdx+1; j<numPiles; j++) {
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

        this->updateSystemPlot();
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

        systemPlot->setActivePile(numPiles);
    }
    else
    {
        QMessageBox msg(QMessageBox::Information,"Info","Maximum number of piles reached.");
        msg.exec();
    }

    ui->pileIndex->setMaximum(numPiles);
    ui->pileIndex->setValue(numPiles);

    this->updateSystemPlot();
    this->doAnalysis();
}

void MainWindow::on_xOffset_valueChanged(double arg1)
{
    int pileIdx = ui->pileIndex->value() - 1;
    xOffset[pileIdx] = ui->xOffset->value();

    this->doAnalysis();
    this->updateSystemPlot();
}

void MainWindow::on_pileIndex_valueChanged(int arg1)
{
    int pileIdx = ui->pileIndex->value() - 1;

    ui->pileDiameter->setValue(pileDiameter[pileIdx]);
    ui->Emodulus->setValue( (E[pileIdx]/1.0e+6) );
    ui->embeddedLength->setValue(L2[pileIdx]);
    ui->freeLength->setValue(L1);
    ui->xOffset->setValue(xOffset[pileIdx]);

    // refresh graphics

    activePileIdx  = pileIdx;
    activeLayerIdx = -1;

    this->updateSystemPlot();
}

/* ***** pile parameter changes ***** */

void MainWindow::on_pileDiameter_valueChanged(double arg1)
{
    int pileIdx = ui->pileIndex->value() - 1;

    pileDiameter[pileIdx] = arg1;
    this->doAnalysis();
    this->updateSystemPlot();
}

void MainWindow::on_embeddedLength_valueChanged(double arg1)
{
    int pileIdx = ui->pileIndex->value() - 1;

    L2[pileIdx] = arg1;
    this->doAnalysis();
    this->updateSystemPlot();
}

void MainWindow::on_freeLength_valueChanged(double arg1)
{
    L1 = arg1;
    this->doAnalysis();
    this->updateSystemPlot();
}

void MainWindow::on_Emodulus_valueChanged(double arg1)
{
    int pileIdx = ui->pileIndex->value() - 1;

    E[pileIdx] = arg1*1.0e6;
    this->doAnalysis();
    this->updateSystemPlot();
}

void MainWindow::on_groundWaterTable_valueChanged(double arg1)
{
    gwtDepth = arg1;
    this->doAnalysis();
    this->updateSystemPlot();
}


//
// layer properties and setup methods
//

void MainWindow::setActiveLayer(int layerIdx)
{
    ui->chkBox_layer1->setChecked((layerIdx==0));
    ui->chkBox_layer2->setChecked((layerIdx==1));
    ui->chkBox_layer3->setChecked((layerIdx==2));

    inSetupState = true;  // temporary deactivate live recomputation

    ui->layerThickness->setValue(mSoilLayers[layerIdx].getLayerThickness());
    ui->layerDryWeight->setValue(mSoilLayers[layerIdx].getLayerUnitWeight());
    ui->layerSaturatedWeight->setValue(mSoilLayers[layerIdx].getLayerSatUnitWeight());
    ui->layerFrictionAngle->setValue(mSoilLayers[layerIdx].getLayerFrictionAng());
    ui->layerShearModulus->setValue((mSoilLayers[layerIdx].getLayerStiffness()/1.e3));

    inSetupState = false;

    activePileIdx  = -1;
    activeLayerIdx = layerIdx;

    this->updateSystemPlot();
}

void MainWindow::on_chkBox_layer1_clicked()
{
    setActiveLayer(0);
}

void MainWindow::on_chkBox_layer2_clicked()
{
    setActiveLayer(1);
}

void MainWindow::on_chkBox_layer3_clicked()
{
    setActiveLayer(2);
}

int MainWindow::findActiveLayer()
{
    int layerIdx = -1;
    if (ui->chkBox_layer1->checkState() == Qt::Checked) layerIdx = 0;
    if (ui->chkBox_layer2->checkState() == Qt::Checked) layerIdx = 1;
    if (ui->chkBox_layer3->checkState() == Qt::Checked) layerIdx = 2;

    activePileIdx  = -1;
    activeLayerIdx = layerIdx;

    return layerIdx;
}

void MainWindow::on_layerThickness_valueChanged(double arg1)
{
    double val;
    int layerIdx = findActiveLayer();

    if (arg1 < 0.10) {
        val = 0.10;
        ui->layerThickness->setValue(val);
    }
    else {
        val = arg1;
    }
    if (layerIdx >= 0) {
        mSoilLayers[layerIdx].setLayerThickness(val);
        this->updateLayerState();
        this->doAnalysis();
        this->updateSystemPlot();
    }
}

void MainWindow::on_layerDryWeight_valueChanged(double arg1)
{
    double val;
    int layerIdx = findActiveLayer();

    if (arg1 < 0.5*GAMMA_WATER) {
        val = 0.5*GAMMA_WATER;
        ui->layerDryWeight->setValue(val);
    }
    else {
        val = arg1;
    }
    if (layerIdx >= 0) {
        mSoilLayers[layerIdx].setLayerUnitWeight(val);
        this->updateLayerState();
        this->doAnalysis();
        this->updateSystemPlot();
    }
}

void MainWindow::on_layerSaturatedWeight_valueChanged(double arg1)
{
    double val;
    int layerIdx = findActiveLayer();

    if (arg1 < GAMMA_WATER) {
        val = GAMMA_WATER;
        ui->layerSaturatedWeight->setValue(val);
    }
    else {
        val = arg1;
    }
    if (layerIdx >= 0) {
        mSoilLayers[layerIdx].setLayerSatUnitWeight(val);
        this->updateLayerState();
        this->doAnalysis();
        this->updateSystemPlot();
    }
}

void MainWindow::on_layerFrictionAngle_valueChanged(double arg1)
{
    double val;
    int layerIdx = findActiveLayer();

    if (arg1 < 1.0) {
        val = 1.0;
        ui->layerFrictionAngle->setValue(val);
    }
    else {
        val = arg1;
    }
    if (layerIdx >= 0) {
        mSoilLayers[layerIdx].setLayerFrictionAng(val);
        this->updateLayerState();
        this->doAnalysis();
        this->updateSystemPlot();
    }
}

void MainWindow::on_layerShearModulus_valueChanged(double arg1)
{
    double val;
    int layerIdx = findActiveLayer();

    if (arg1 < 1.0) {
        val = 1.0e3;
        ui->layerShearModulus->setValue(1.0);
    }
    else {
        val = arg1*1.0e3;
    }
    if (layerIdx >= 0) {
        mSoilLayers[layerIdx].setLayerStiffness( (val) );
        this->updateLayerState();
        this->doAnalysis();
        this->updateSystemPlot();
    }
}

int  MainWindow::adjustLayersToPiles()
{
    return 0;
}

void MainWindow::on_properties_currentChanged(int index)
{
    switch (index) {
    case 0:
        activePileIdx  = ui->pileIndex->value() - 1;
        activeLayerIdx = -1;
        break;
    case 1:
        activePileIdx  = -1;
        this->findActiveLayer();
        break;
    default:
        activePileIdx  = -1;
        activeLayerIdx = -1;
    }

    this->updateSystemPlot();
}

void MainWindow::updateSystemPlot()
{
    if (inSetupState) return;

    //
    // create pile information for plotting
    //
    QVector<PILE_INFO> pileInfo;

    pileInfo.clear();

    for (int i=0; i<numPiles; i++)
    {
        PILE_INFO thisPile;
        thisPile.L1           = L1;
        thisPile.L2           = L2[i];
        thisPile.pileDiameter = pileDiameter[i];
        thisPile.xOffset      = xOffset[i];
        thisPile.E            = E[i];
        pileInfo.append(thisPile);
    }
    systemPlot->updatePiles(pileInfo);

    //
    // create list of layer interface positions
    //
    this->updateLayerState();

    QVector<double> layerDepth(MAXLAYERS+1, 0.00);
    for (int i=1; i<=MAXLAYERS; i++)
    {
        if (mSoilLayers.size() >= i)
            { layerDepth[i] = mSoilLayers[i-1].getLayerDepth() + mSoilLayers[i-1].getLayerThickness(); }
        else
            { layerDepth[i] = layerDepth[i-1]; }
    }
    systemPlot->updateSoil(layerDepth);

    this->updateSoilResultPlots(layerDepth);

    //displPlot->updateSoil(layerDepth);
    //pullOutPlot->updateSoil(layerDepth);
    //momentPlot->updateSoil(layerDepth);
    //shearPlot->updateSoil(layerDepth);
    //axialPlot->updateSoil(layerDepth);
    //stressPlot->updateSoil(layerDepth);
    //pultPlot->updateSoil(layerDepth);
    //y50Plot->updateSoil(layerDepth);
    //tultPlot->updateSoil(layerDepth);
    //z50Plot->updateSoil(layerDepth);

    //
    // update ground water table
    //
    systemPlot->updateGWtable(gwtDepth);

    //
    // set the load control type
    //
    systemPlot->setLoadType(loadControlType);

    //
    // update the load control data
    //
    double pltSurfaceDisp    = 0.0;
    double HPush  = 0.0;
    double VPush  = 0.0;
    double FHorz  = 0.0;
    double FVert  = 0.0;
    double Moment = 0.0;

    switch (loadControlType) {
    case LoadControlType::ForceControl:
        FHorz  = P;
        FVert  = PV;
        Moment = PMom;
        break;
    case LoadControlType::PushOver:
        HPush = HDisp;
        VPush = VDisp;
        break;
    case LoadControlType::SoilMotion:
        pltSurfaceDisp = surfaceDisp;
        break;
    }
    systemPlot->updateLoad(FHorz, FVert, Moment);
    systemPlot->updateDisplacement(HPush, VPush);
    systemPlot->updateDispProfile(pltSurfaceDisp, percentage12, percentage23, percentageBase);

    //
    // refresh the plot
    //
    systemPlot->refresh();
    this->refreshResultPlots();
}

void MainWindow::updateSoilResultPlots(QVector<double> &layerDepth)
{
    displPlot->updateSoil(layerDepth);
    pullOutPlot->updateSoil(layerDepth);
    momentPlot->updateSoil(layerDepth);
    shearPlot->updateSoil(layerDepth);
    axialPlot->updateSoil(layerDepth);
    stressPlot->updateSoil(layerDepth);
    pultPlot->updateSoil(layerDepth);
    y50Plot->updateSoil(layerDepth);
    tultPlot->updateSoil(layerDepth);
    z50Plot->updateSoil(layerDepth);
}

void MainWindow::refreshResultPlots()
{
    displPlot->refresh();
    pullOutPlot->refresh();
    momentPlot->refresh();
    shearPlot->refresh();
    axialPlot->refresh();
    stressPlot->refresh();
    pultPlot->refresh();
    y50Plot->refresh();
    tultPlot->refresh();
    z50Plot->refresh();
}

void MainWindow::on_layerSelectedInSystemPlot(bool selected)
{
    qDebug() << "on_layerSelectedInSystemPlot(" << selected << ") triggered";
}


void MainWindow::onSystemPlot_pileSelected(int index)
{
    activePileIdx  = index;
    activeLayerIdx = -1;

    // make pile controls visible
    ui->properties->setCurrentWidget(ui->pilePropertiesWidget);
    ui->pileIndex->setValue(index+1);
}

void MainWindow::onSystemPlot_soilLayerSelected(int index)
{
    activePileIdx  = -1;
    activeLayerIdx = index;

    // make soil layer controls visible
    ui->properties->setCurrentWidget(ui->soilPropertiesWidget);
    setActiveLayer(index);
}

void MainWindow::onSystemPlot_groundWaterSelected()
{
    // make groundwater settings visible:
    ui->properties->setCurrentWidget(ui->soilPropertiesWidget);
}

void MainWindow::on_actionLicense_Information_triggered()
{
    CopyrightDialog *dlg = new CopyrightDialog(this);

    //
    // adjust size of application window to the available display
    //
    QRect rec = QApplication::desktop()->screenGeometry();
    int height = 0.50*rec.height();
    int width  = 0.50*rec.width();
    dlg->resize(width, height);

    dlg->exec();
}

void MainWindow::on_actionLicense_triggered()
{
    this->on_actionLicense_Information_triggered();
}

void MainWindow::on_actionProvide_Feedback_triggered()
{
  QDesktopServices::openUrl(QUrl("https://www.designsafe-ci.org/help/new-ticket/", QUrl::TolerantMode));

}

bool MainWindow::ReadFile(QString s)
{
    /* identify filename and location for loading */

    QString theFolder = QDir::homePath();
    QString theFilter = "Json file (*.json)";
    QFileDialog dlg;

    QString filename = dlg.getOpenFileName(this, "Load file", theFolder, theFilter);

    qWarning() << filename;

    /* load JSON object from file */
    QFile loadFile;
    loadFile.setFileName(filename);

    if (!loadFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Couldn't open load file.");
        return false;
    }

    QString theFile = loadFile.readAll();
    loadFile.close();

    bool fileTypeError = false;

    QJsonDocument infoDoc = QJsonDocument::fromJson(theFile.toUtf8());

    /* start a JSON object to represent the system */
    QJsonObject json = infoDoc.object();

    QString creator;
    creator  = json["creator"].toString();

    if (creator != "PileGroupTool") fileTypeError = true;

    QString version;
    version  = json["version"].toString();

    fileTypeError = true;
    if (version.startsWith("1.") ) fileTypeError = false;
    if (version.startsWith("2.0") ) fileTypeError = false;
    if (version.startsWith("2.1") ) fileTypeError = false;

    if (fileTypeError) {
        QMessageBox msg(QMessageBox::Information, "Info", "Not a valid model file.");
        msg.exec();
        return false;
    }

    QString username;
    username = json["username"].toString();
    QString author;
    author   = json["author"].toString();
    QString filedate;
    filedate = json["date"].toString();

    /* write layer information */
    QJsonArray layerInfo = json["layers"].toArray();

    int nLayer = 0;

    foreach (QJsonValue jval, layerInfo) {

        QJsonObject aLayer = jval.toObject();

        mSoilLayers[nLayer].setLayerDepth(aLayer["depth"].toDouble());
        mSoilLayers[nLayer].setLayerThickness(aLayer["thickness"].toDouble());
        mSoilLayers[nLayer].setLayerUnitWeight(aLayer["gamma"].toDouble());
        mSoilLayers[nLayer].setLayerSatUnitWeight(aLayer["gammaSaturated"].toDouble());
        mSoilLayers[nLayer].setLayerFrictionAng(aLayer["phi"].toDouble());
        mSoilLayers[nLayer].setLayerCohesion(aLayer["cohesion"].toDouble());
        mSoilLayers[nLayer].setLayerStiffness(aLayer["Gmodulus"].toDouble());

        nLayer++;
        if (nLayer >= MAXLAYERS)  break;
    }

    while (nLayer+1 < MAXLAYERS) {
        /* fill layer array with identical layer properties */
        mSoilLayers[nLayer+1].setLayerDepth(mSoilLayers[nLayer].getLayerDepth());
        mSoilLayers[nLayer+1].setLayerThickness(mSoilLayers[nLayer].getLayerThickness());
        mSoilLayers[nLayer+1].setLayerUnitWeight(mSoilLayers[nLayer].getLayerUnitWeight());
        mSoilLayers[nLayer+1].setLayerSatUnitWeight(mSoilLayers[nLayer].getLayerSatUnitWeight());
        mSoilLayers[nLayer+1].setLayerFrictionAng(mSoilLayers[nLayer].getLayerFrictionAng());
        mSoilLayers[nLayer+1].setLayerCohesion(mSoilLayers[nLayer].getLayerCohesion());
        mSoilLayers[nLayer+1].setLayerStiffness(mSoilLayers[nLayer].getLayerStiffness());

        nLayer++;
    }

    double groundWaterTable = json["groundWaterTable"].toDouble();
    if (groundWaterTable < 0.0) groundWaterTable = 0.0;
    gwtDepth = groundWaterTable;

    /* write pile information */
    QJsonArray pileInfo = json["piles"].toArray();

    int nPile = 0;

    foreach (QJsonValue jval, pileInfo) {
        QJsonObject aPile = jval.toObject();

        L2[nPile]           = aPile["embeddedLength"].toDouble();
        L1                  = aPile["freeLength"].toDouble();
        pileDiameter[nPile] = aPile["diameter"].toDouble();
        E[nPile]            = aPile["YoungsModulus"].toDouble();
        xOffset[nPile]      = aPile["xOffset"].toDouble();

        nPile++;
        if (nPile >= MAXPILES) break;
    }

    numPiles = nPile;

    useToeResistance              = json["useToeResistance"].toBool();
    assumeRigidPileHeadConnection = json["assumeRigidPileHeadConnection"].toBool();


    /* read load information */
    if (version.startsWith("1.0"))
    {
        QJsonObject loadInfo = json["loads"].toObject();

        P    = loadInfo["HForce"].toDouble();
        PV   = loadInfo["VForce"].toDouble();
        PMom = loadInfo["Moment"].toDouble();

        HDisp = 0.0;
        VDisp = 0.0;

        surfaceDisp  = 0.0;
        percentage12 = 100.;
        percentage23 = 0.0;
        percentageBase = 0.0;

        loadControlType = LoadControlType::ForceControl;
    }
    else if (version.startsWith("1.99") || version.startsWith("2.") )
    {
        QJsonObject loadInfo = json["loads"].toObject();
        QJsonObject forceControlObj = loadInfo["forceControl"].toObject();
        QJsonObject pushOverObj     = loadInfo["pushOver"].toObject();
        QJsonObject soilMotionObj   = loadInfo["soilMotion"].toObject();

        QString loadType = loadInfo["loadControlType"].toString();

        if (loadType.toLower() == "forcecontrol")
            { loadControlType = LoadControlType::ForceControl; }
        else if (loadType.toLower() == "pushover")
            { loadControlType = LoadControlType::PushOver; }
        else if (loadType.toLower() == "soilmotion")
            { loadControlType = LoadControlType::SoilMotion; }
        else
            { loadControlType = LoadControlType::ForceControl; }

        P    = forceControlObj["HForce"].toDouble();
        PV   = forceControlObj["VForce"].toDouble();
        PMom = forceControlObj["Moment"].toDouble();

        HDisp = pushOverObj["HDisp"].toDouble();
        VDisp = pushOverObj["VDisp"].toDouble();

        surfaceDisp    = soilMotionObj["surfaceDisp"].toDouble();
        percentage12   = soilMotionObj["percentage12"].toDouble();
        percentage23   = soilMotionObj["percentage23"].toDouble();
        percentageBase = soilMotionObj["percentageBase"].toDouble();
    }

    /* FEA parameters */
    QJsonObject FEAparameters = json["FEAparameters"].toObject();

    minElementsPerLayer = FEAparameters["minElementsPerLayer"].toInt();
    maxElementsPerLayer = FEAparameters["maxElementsPerLayer"].toInt();
    numElementsInAir    = FEAparameters["numElementsInAir"].toInt();

    if (minElementsPerLayer < MIN_ELEMENTS_PER_LAYER)   minElementsPerLayer = MIN_ELEMENTS_PER_LAYER;
    if (maxElementsPerLayer > 3*MAX_ELEMENTS_PER_LAYER) maxElementsPerLayer = 3*MAX_ELEMENTS_PER_LAYER;
    if (numElementsInAir < NUM_ELEMENTS_IN_AIR)         numElementsInAir = NUM_ELEMENTS_IN_AIR;
    if (numElementsInAir > MAX_ELEMENTS_PER_LAYER)      numElementsInAir = MAX_ELEMENTS_PER_LAYER;

    return true;
}

bool MainWindow::WriteFile(QString s)
{
    /* identify filename and location for saving */

    QString path = QDir::homePath();
    QDir d;
    d.setPath(path);
    QString filename = d.filePath("PileGroupTool.json");
    QString theFilter = "Json file (*.json)";
    QFileDialog dlg;

    filename = dlg.getSaveFileName(this, "Save file", filename, theFilter );

    // check if cancelled
    if (filename.isEmpty()) return false;

    /* start a JSON object to represent the system */
    QJsonObject *json = new QJsonObject();

    json->insert("creator", QString("PileGroupTool"));
    json->insert("version", QString(APP_VERSION));
#ifdef Q_OS_WIN
    QString username = qgetenv("USERNAME");
#else
    QString username = qgetenv("USER");
#endif
    json->insert("author", username);
    json->insert("date", QDateTime::currentDateTime().toString());

    /* write layer information */
    QJsonArray *layerInfo = new QJsonArray();
    for (int lid=0; lid<MAXLAYERS; lid++) {
        QJsonObject aLayer;

        aLayer.insert("depth", mSoilLayers[lid].getLayerDepth());
        aLayer.insert("thickness", mSoilLayers[lid].getLayerThickness());
        aLayer.insert("gamma", mSoilLayers[lid].getLayerUnitWeight());
        aLayer.insert("gammaSaturated", mSoilLayers[lid].getLayerSatUnitWeight());
        aLayer.insert("phi", mSoilLayers[lid].getLayerFrictionAng());
        aLayer.insert("cohesion", mSoilLayers[lid].getLayerCohesion());
        aLayer.insert("Gmodulus", mSoilLayers[lid].getLayerStiffness());

        layerInfo->append(aLayer);
    }

    json->insert("layers", *layerInfo);

    json->insert("groundWaterTable", gwtDepth);

    /* write pile information */
    QJsonArray *pileInfo = new QJsonArray();
    for (int pid=0; pid<numPiles; pid++) {
        QJsonObject aPile;

        aPile.insert("embeddedLength", L2[pid]);
        aPile.insert("freeLength", L1);
        aPile.insert("diameter", pileDiameter[pid]);
        aPile.insert("YoungsModulus", E[pid]);
        aPile.insert("xOffset", xOffset[pid]);

        pileInfo->append(aPile);
    }

    json->insert("piles", *pileInfo);

    json->insert("useToeResistance", useToeResistance);
    json->insert("assumeRigidPileHeadConnection", assumeRigidPileHeadConnection);


    /* write load information */
    QJsonObject *loadInfo = new QJsonObject();

    int selection = ui->forceTypeSelector->currentIndex();
    QString loadType;

    switch (selection)
    {
    case 0:
        loadType = "forceControl";
        break;
    case 1:
        loadType = "pushOver";
        break;
    case 2:
        loadType = "soilMotion";
        break;
    default:
        qWarning() << "cannot identify the loadControlType";
    }

    loadInfo->insert("loadControlType", loadType);

    QJsonObject *forceControlObj = new QJsonObject();
    forceControlObj->insert("HForce", P);
    forceControlObj->insert("VForce", PV);
    forceControlObj->insert("Moment", PMom);
    loadInfo->insert("forceControl", *forceControlObj);

    QJsonObject *pushOverObj     = new QJsonObject();
    pushOverObj->insert("HDisp", HDisp);
    pushOverObj->insert("VDisp", VDisp);
    loadInfo->insert("pushOver", *pushOverObj);

    QJsonObject *soilMotionObj   = new QJsonObject();
    soilMotionObj->insert("surfaceDisp", surfaceDisp);
    soilMotionObj->insert("percentage12", percentage12);
    soilMotionObj->insert("percentage23", percentage23);
    soilMotionObj->insert("percentageBase", percentageBase);
    loadInfo->insert("soilMotion", *soilMotionObj);

    json->insert("loads", *loadInfo);

    /* FEA parameters */
    QJsonObject FEAparameters;

    FEAparameters.insert("minElementsPerLayer",minElementsPerLayer);
    FEAparameters.insert("maxElementsPerLayer",maxElementsPerLayer);
    FEAparameters.insert("numElementsInAir", numElementsInAir);

    json->insert("FEAparameters", FEAparameters);

    QJsonDocument infoDoc = QJsonDocument(*json);

    /* write JSON object to file */

    QFile saveFile( filename );

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    saveFile.write( infoDoc.toJson() );
    saveFile.close();

    // clean up
    delete layerInfo;
    delete pileInfo;
    delete loadInfo;
    delete forceControlObj;
    delete pushOverObj;
    delete soilMotionObj;
    delete json;

    return true;
}


void MainWindow::replyFinished(QNetworkReply *pReply)
{
    return;
}


void MainWindow::on_forceTypeSelector_activated(int index)
{
    ui->loadTypesStack->setCurrentIndex(index);

    if (index == 0)
    {
        loadControlType = LoadControlType::ForceControl;
        ui->HForceCtrl->setValue(std::nearbyint(P));
        ui->VForceCtrl->setValue(std::nearbyint(PV));
        ui->MomentCtrl->setValue(std::nearbyint(PMom));
    }
    if (index == 1)
    {
        loadControlType = LoadControlType::PushOver;
        ui->PushOverCtrl->setValue(std::nearbyint(HDisp*1000));
        ui->PullOutCtrl->setValue(std::nearbyint(VDisp*1000));
    }
    if (index == 2)
    {
        loadControlType = LoadControlType::SoilMotion;
        ui->SurfaceDisplacementCtrl->setValue(std::nearbyint(surfaceDisp*1000));
        ui->Interface12Ctrl->setValue(std::nearbyint(100.0*percentage12));
        ui->Interface23Ctrl->setValue(std::nearbyint(100.0*percentage23));
        ui->BaseDisplacementCtrl->setValue(std::nearbyint(100.0*percentageBase));
    }

    systemPlot->setLoadType(loadControlType);
    pileFEAmodel->setLoadType(loadControlType);
    this->doAnalysis();
}


/* ***** loading parameter changes ***** */

void MainWindow::on_HForceCtrl_valueChanged(int value)
{
    P = double(value);

    this->doAnalysis();
    this->updateSystemPlot();
}

void MainWindow::on_VForceCtrl_valueChanged(int value)
{
    PV = double(value);

    this->doAnalysis();
    this->updateSystemPlot();
}

void MainWindow::on_MomentCtrl_valueChanged(int value)
{
    PMom = double(value);

    this->doAnalysis();
    this->updateSystemPlot();
}




void MainWindow::on_PushOverCtrl_valueChanged(int value)
{
    HDisp = double(value)/1000.;

    this->doAnalysis();
    this->updateSystemPlot();
}

void MainWindow::on_PullOutCtrl_valueChanged(int value)
{
    VDisp = double(value)/1000.;

    this->doAnalysis();
    this->updateSystemPlot();
}



void MainWindow::on_SurfaceDisplacementCtrl_valueChanged(int value)
{
    surfaceDisp = double(value)/1000.;

    systemPlot->updateDispProfile(surfaceDisp, percentage12, percentage23, percentageBase);
    this->doAnalysis();
    this->updateSystemPlot();
}

void MainWindow::on_Interface12Ctrl_valueChanged(int value)
{
    percentage12 = double(value)/100.;

    systemPlot->updateDispProfile(surfaceDisp, percentage12, percentage23, percentageBase);
    this->doAnalysis();
    this->updateSystemPlot();
}

void MainWindow::on_Interface23Ctrl_valueChanged(int value)
{
    percentage23 = double(value)/100.;

    systemPlot->updateDispProfile(surfaceDisp, percentage12, percentage23, percentageBase);
    this->doAnalysis();
    this->updateSystemPlot();
}

void MainWindow::on_BaseDisplacementCtrl_valueChanged(int value)
{
    percentageBase = double(value)/100.;

    systemPlot->updateDispProfile(surfaceDisp, percentage12, percentage23, percentageBase);
    this->doAnalysis();
    this->updateSystemPlot();
}



void MainWindow::on_actionQuick_Tips_triggered()
{
    DialogQuickTips *dlg = new DialogQuickTips();

    //
    // adjust size of application window to the available display
    //
    QRect rec = QApplication::desktop()->screenGeometry();
    int height = 0.50*rec.height();
    int width  = 0.50*rec.width();
    dlg->resize(width, height);

    dlg->exec();
    delete dlg;
}
