#include "dialogpreferences.h"
#include "ui_dialogpreferences.h"

#include <QSettings>
#include <QApplication>
#include <QTabBar>

#include <qdebug.h>

DialogPreferences::DialogPreferences(QWidget *parent, QSettings *settings) :
    QDialog(parent),
    ui(new Ui::DialogPreferences)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->setTabEnabled(2,false);

    //
    // initialize the setup file
    //

    if (settings != nullptr) {
        DLGsettings = settings;
    }
    else {
        DLGsettings = new QSettings("NHERI SimCenter","Pile Group Tool");
    }

    qDebug() << DLGsettings->fileName();

    DLGsettings->beginGroup("general");
        DLGgraphicsLib       = DLGsettings->value("graphicsLibrary","QwtAll").toString();
        if (DLGgraphicsLib == "QCP") { DLGgraphicsLib="QwtAll"; }
        if (DLGgraphicsLib == "Qwt") { DLGgraphicsLib="QwtAll"; }
        DLGfemAnalyzer       = DLGsettings->value("femAnalyzer","OpenSeesInt").toString();
    DLGsettings->endGroup();

    DLGsettings->beginGroup("viewer");
        DLGshowDisplacements = DLGsettings->value("displacements",1).toInt();
        DLGshowPullOut       = DLGsettings->value("pullout",1).toInt();
        DLGshowMoments       = DLGsettings->value("moments",1).toInt();
        DLGshowShear         = DLGsettings->value("shear",1).toInt();
        DLGshowAxial         = DLGsettings->value("axial",1).toInt();
        DLGshowStress        = DLGsettings->value("stress",1).toInt();
        DLGshowPultimate     = DLGsettings->value("pult",1).toInt();
        DLGshowY50           = DLGsettings->value("compliance",1).toInt();
        DLGshowTultimate     = DLGsettings->value("tult",1).toInt();
        DLGshowZ50           = DLGsettings->value("Zcompliance",1).toInt();
    DLGsettings->endGroup();

    // meshing parameters
    DLGsettings->beginGroup("fea");
        DLGminElementsPerLayer = DLGsettings->value("minElemementsPerLayer",15).toInt();
        DLGmaxElementsPerLayer = DLGsettings->value("maxElemementsPerLayer",40).toInt();
        DLGnumElementsInAir    = DLGsettings->value("numElementsInAir", 4).toInt();
        DLGmaxNumberOfPiles    = DLGsettings->value("maxNumberOfPiles", 3).toInt();
    DLGsettings->endGroup();

    //
    // set states in dialog according to presets read from the setup file
    //

    this->InitGUI();
}

DialogPreferences::~DialogPreferences()
{
    delete ui;
}

void DialogPreferences::InitGUI()
{
    if (DLGgraphicsLib == "QCP" )
        { ui->rbtn_useQCP->setChecked(true); }
    else
        { ui->rbtn_useQwt->setChecked(true); }  // default to Qwt

    if (DLGfemAnalyzer == "OpenSeesInt") { ui->rbtn_OpenSeesInt->setChecked(true); }
    if (DLGfemAnalyzer == "OpenSeesExt") { ui->rbtn_OpenSeesExt->setChecked(true); }

    ui->chkBx_displacement->setCheckState(DLGshowDisplacements!=0?Qt::Checked:Qt::Unchecked);
    ui->chkBx_pullOut->setCheckState(DLGshowPullOut!=0?Qt::Checked:Qt::Unchecked);
    ui->chkBx_moment->setCheckState(DLGshowMoments!=0?Qt::Checked:Qt::Unchecked);
    ui->chkBx_shear->setCheckState(DLGshowShear!=0?Qt::Checked:Qt::Unchecked);
    ui->chkBx_axialForce->setCheckState(DLGshowAxial!=0?Qt::Checked:Qt::Unchecked);
    ui->chkBx_stress->setCheckState(DLGshowStress!=0?Qt::Checked:Qt::Unchecked);
    ui->chkBx_pu->setCheckState(DLGshowPultimate!=0?Qt::Checked:Qt::Unchecked);
    ui->chkBx_y50->setCheckState(DLGshowY50!=0?Qt::Checked:Qt::Unchecked);
    ui->chkBx_tu->setCheckState(DLGshowTultimate!=0?Qt::Checked:Qt::Unchecked);
    ui->chkBx_z50->setCheckState(DLGshowZ50!=0?Qt::Checked:Qt::Unchecked);

    ui->elementsAboveGround->setValue(DLGnumElementsInAir);
    ui->minElementsPerLayer->setValue(DLGminElementsPerLayer);
    ui->maxElementsPerLayer->setValue(DLGmaxElementsPerLayer);
}

void DialogPreferences::on_buttonBox_clicked(QAbstractButton *button)
{
    // get here on OK and on Reset
    if (button->text() != "Reset") return;

    DLGsettings->beginGroup("general");
        if (ui->rbtn_useQCP->isChecked()) {DLGgraphicsLib = QString("QCP");}
        if (ui->rbtn_useQwt->isChecked()) {DLGgraphicsLib = QString("QwtAll");}

        if (ui->rbtn_OpenSeesInt->isChecked()) {DLGfemAnalyzer = QString("OpenSeesInt");}
        if (ui->rbtn_OpenSeesExt->isChecked()) {DLGfemAnalyzer = QString("OpenSeesExt");}

        DLGsettings->setValue("graphicsLibrary",DLGgraphicsLib);
        DLGsettings->setValue("femAnalyzer",DLGfemAnalyzer);
    DLGsettings->endGroup();

    DLGsettings->beginGroup("viewer");
        DLGshowDisplacements = 1;
        DLGshowPullOut       = 1;
        DLGshowMoments       = 1;
        DLGshowShear         = 1;
        DLGshowAxial         = 1;
        DLGshowStress        = 1;
        DLGshowPultimate     = 1;
        DLGshowY50           = 1;
        DLGshowTultimate     = 1;
        DLGshowZ50           = 1;

        DLGsettings->setValue("displacements",DLGshowDisplacements);
        DLGsettings->setValue("pullout",DLGshowPullOut);
        DLGsettings->setValue("moments",DLGshowMoments);
        DLGsettings->setValue("shear",DLGshowShear);
        DLGsettings->setValue("axial",DLGshowAxial);
        DLGsettings->setValue("stress",DLGshowStress);
        DLGsettings->setValue("pult",DLGshowPultimate);
        DLGsettings->setValue("compliance",DLGshowY50);
        DLGsettings->setValue("tult",DLGshowTultimate);
        DLGsettings->setValue("Zcompliance",DLGshowZ50);
    DLGsettings->endGroup();

    // meshing parameters
    DLGsettings->beginGroup("fea");
        DLGminElementsPerLayer = 15;
        DLGmaxElementsPerLayer = 40;
        DLGnumElementsInAir    =  4;

        DLGsettings->setValue("minElemementsPerLayer",DLGminElementsPerLayer);
        DLGsettings->setValue("maxElemementsPerLayer",DLGmaxElementsPerLayer);
        DLGsettings->setValue("numElementsInAir",DLGnumElementsInAir);
        DLGsettings->setValue("maxNumberOfPiles",DLGmaxNumberOfPiles);
    DLGsettings->endGroup();

    this->InitGUI();
}

void DialogPreferences::on_chkBx_displacement_stateChanged(int arg1)
{
    DLGshowDisplacements = arg1==Qt::Checked?1:0;
    DLGsettings->beginGroup("viewer");
        DLGsettings->setValue("displacements",DLGshowDisplacements);
    DLGsettings->endGroup();
}

void DialogPreferences::on_chkBx_pullOut_stateChanged(int arg1)
{
    DLGshowPullOut = arg1==Qt::Checked?1:0;
    DLGsettings->beginGroup("viewer");
        DLGsettings->setValue("pullout",DLGshowPullOut);
    DLGsettings->endGroup();
}

void DialogPreferences::on_chkBx_moment_stateChanged(int arg1)
{
    DLGshowMoments = arg1==Qt::Checked?1:0;
    DLGsettings->beginGroup("viewer");
        DLGsettings->setValue("moments",DLGshowMoments);
    DLGsettings->endGroup();
}

void DialogPreferences::on_chkBx_shear_stateChanged(int arg1)
{
    DLGshowShear = arg1==Qt::Checked?1:0;
    DLGsettings->beginGroup("viewer");
        DLGsettings->setValue("shear",DLGshowShear);
    DLGsettings->endGroup();
}

void DialogPreferences::on_chkBx_axialForce_stateChanged(int arg1)
{
    DLGshowAxial = arg1==Qt::Checked?1:0;
    DLGsettings->beginGroup("viewer");
        DLGsettings->setValue("axial",DLGshowAxial);
    DLGsettings->endGroup();
}

void DialogPreferences::on_chkBx_stress_stateChanged(int arg1)
{
    DLGshowStress = arg1==Qt::Checked?1:0;
    DLGsettings->beginGroup("viewer");
        DLGsettings->setValue("stress",DLGshowStress);
    DLGsettings->endGroup();
}

void DialogPreferences::on_chkBx_pu_stateChanged(int arg1)
{
    DLGshowPultimate = arg1==Qt::Checked?1:0;
    DLGsettings->beginGroup("viewer");
        DLGsettings->setValue("pult",DLGshowPultimate);
    DLGsettings->endGroup();
}

void DialogPreferences::on_chkBx_y50_stateChanged(int arg1)
{
    DLGshowY50 = arg1==Qt::Checked?1:0;
    DLGsettings->beginGroup("viewer");
        DLGsettings->setValue("compliance",DLGshowY50);
    DLGsettings->endGroup();
}

void DialogPreferences::on_chkBx_tu_stateChanged(int arg1)
{
    DLGshowTultimate = arg1==Qt::Checked?1:0;
    DLGsettings->beginGroup("viewer");
        DLGsettings->setValue("tult",DLGshowTultimate);
    DLGsettings->endGroup();
}

void DialogPreferences::on_chkBx_z50_stateChanged(int arg1)
{
    DLGshowZ50 = arg1==Qt::Checked?1:0;
    DLGsettings->beginGroup("viewer");
        DLGsettings->setValue("Zcompliance",DLGshowZ50);
    DLGsettings->endGroup();
}

/* general settings block */

void DialogPreferences::on_rbtn_useQCP_clicked(bool checked)
{
    if (!checked) return;

    DLGgraphicsLib = QString("QCP");
    DLGsettings->beginGroup("general");
        DLGsettings->setValue("graphicsLibrary",DLGgraphicsLib);
    DLGsettings->endGroup();
}

void DialogPreferences::on_rbtn_useQwt_clicked(bool checked)
{
    if (!checked) return;

    //DLGgraphicsLib = QString("QwtSystem");
    DLGgraphicsLib = QString("QwtAll");
    DLGsettings->beginGroup("general");
        DLGsettings->setValue("graphicsLibrary",DLGgraphicsLib);
    DLGsettings->endGroup();
}

void DialogPreferences::on_rbtn_OpenSeesInt_clicked(bool checked)
{
    if (!checked) return;

    DLGfemAnalyzer = QString("OpenSeesInt");
    DLGsettings->beginGroup("general");
        DLGsettings->setValue("femAnalyzer",DLGfemAnalyzer);
    DLGsettings->endGroup();
}

void DialogPreferences::on_rbtn_OpenSeesExt_clicked(bool checked)
{
    if (!checked) return;

    DLGfemAnalyzer = QString("OpenSeesExt");
    DLGsettings->beginGroup("general");
        DLGsettings->setValue("femAnalyzer",DLGfemAnalyzer);
    DLGsettings->endGroup();
}

/* end of general settings */

void DialogPreferences::on_buttonBox_accepted()
{
    //
    // write preferences
    //

    // general settings
    DLGsettings->beginGroup("general");
        DLGsettings->setValue("graphicsLibrary",DLGgraphicsLib);
        DLGsettings->setValue("femAnalyzer",DLGfemAnalyzer);
    DLGsettings->endGroup();

    // viewer settings
    DLGsettings->beginGroup("viewer");
        DLGsettings->setValue("displacements",DLGshowDisplacements);
        DLGsettings->setValue("pullout",DLGshowPullOut);
        DLGsettings->setValue("moments",DLGshowMoments);
        DLGsettings->setValue("shear",DLGshowShear);
        DLGsettings->setValue("axial",DLGshowAxial);
        DLGsettings->setValue("stress",DLGshowStress);
        DLGsettings->setValue("pult",DLGshowPultimate);
        DLGsettings->setValue("compliance",DLGshowY50);
    DLGsettings->endGroup();


    // meshing parameters
    DLGsettings->beginGroup("fea");
        DLGsettings->setValue("minElemementsPerLayer",DLGminElementsPerLayer);
        DLGsettings->setValue("maxElemementsPerLayer",DLGmaxElementsPerLayer);
        DLGsettings->setValue("numElementsInAir",DLGnumElementsInAir);
        DLGsettings->setValue("maxNumberOfPiles",DLGmaxNumberOfPiles);
    DLGsettings->endGroup();
}
