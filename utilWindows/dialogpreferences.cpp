#include "dialogpreferences.h"
#include "ui_dialogpreferences.h"

#include <QSettings>
#include <QApplication>

#include <qdebug.h>

DialogPreferences::DialogPreferences(QWidget *parent, QSettings *settings) :
    QDialog(parent),
    ui(new Ui::DialogPreferences)
{
    ui->setupUi(this);

    //
    // initialize the setup file
    //

    if (settings != NULL) {
        DLGsettings = settings;
    }
    else {
        DLGsettings = new QSettings("NHERI SimCenter","Pile Group Tool");
    }

    qDebug() << DLGsettings->fileName();

    DLGsettings->beginGroup("viewer");
        DLGshowDisplacements = DLGsettings->value("displacements",1).toInt();
        DLGshowMoments       = DLGsettings->value("moments",1).toInt();
        DLGshowShear         = DLGsettings->value("shear",1).toInt();
        DLGshowStress        = DLGsettings->value("stress",1).toInt();
        DLGshowPultimate     = DLGsettings->value("pult",1).toInt();
        DLGshowY50           = DLGsettings->value("compliance",1).toInt();
    DLGsettings->endGroup();

    // meshing parameters
    DLGsettings->beginGroup("fea");
        DLGminElementsPerLayer = DLGsettings->value("minElemementsPerLayer",15).toInt();
        DLGmaxElementsPerLayer = DLGsettings->value("maxElemementsPerLayer",40).toInt();
        DLGnumElementsInAir    = DLGsettings->value("numElementsInAir", 4).toInt();
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
    ui->chkBx_displacement->setCheckState(DLGshowDisplacements!=0?Qt::Checked:Qt::Unchecked);
    ui->chkBx_moment->setCheckState(DLGshowMoments!=0?Qt::Checked:Qt::Unchecked);
    ui->chkBx_shear->setCheckState(DLGshowShear!=0?Qt::Checked:Qt::Unchecked);
    ui->chkBx_stress->setCheckState(DLGshowStress!=0?Qt::Checked:Qt::Unchecked);
    ui->chkBx_pu->setCheckState(DLGshowPultimate!=0?Qt::Checked:Qt::Unchecked);
    ui->chkBx_y50->setCheckState(DLGshowY50!=0?Qt::Checked:Qt::Unchecked);

    ui->elementsAboveGround->setValue(DLGnumElementsInAir);
    ui->minElementsPerLayer->setValue(DLGminElementsPerLayer);
    ui->maxElementsPerLayer->setValue(DLGmaxElementsPerLayer);
}

void DialogPreferences::on_buttonBox_clicked(QAbstractButton *button)
{
    // get here on OK and on Reset
    if (button->text() != "Reset") return;

    DLGsettings->beginGroup("viewer");
        DLGshowDisplacements = 1;
        DLGshowMoments       = 1;
        DLGshowShear         = 1;
        DLGshowStress        = 1;
        DLGshowPultimate     = 1;
        DLGshowY50           = 1;

        DLGsettings->setValue("displacements",DLGshowDisplacements);
        DLGsettings->setValue("moments",DLGshowMoments);
        DLGsettings->setValue("shear",DLGshowShear);
        DLGsettings->setValue("stress",DLGshowStress);
        DLGsettings->setValue("pult",DLGshowPultimate);
        DLGsettings->setValue("compliance",DLGshowY50);
    DLGsettings->endGroup();

    // meshing parameters
    DLGsettings->beginGroup("fea");
        DLGminElementsPerLayer = 15;
        DLGmaxElementsPerLayer = 40;
        DLGnumElementsInAir    =  4;

        DLGsettings->setValue("minElemementsPerLayer",DLGminElementsPerLayer);
        DLGsettings->setValue("maxElemementsPerLayer",DLGmaxElementsPerLayer);
        DLGsettings->setValue("numElementsInAir",DLGnumElementsInAir);
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

void DialogPreferences::on_buttonBox_accepted()
{
    //
    // write preferences
    //
    // viewer settings
    DLGsettings->beginGroup("viewer");
        DLGsettings->setValue("displacements",DLGshowDisplacements);
        DLGsettings->setValue("moments",DLGshowMoments);
        DLGsettings->setValue("shear",DLGshowShear);
        DLGsettings->setValue("stress",DLGshowStress);
        DLGsettings->setValue("pult",DLGshowPultimate);
        DLGsettings->setValue("compliance",DLGshowY50);
    DLGsettings->endGroup();


    // meshing parameters
    DLGsettings->beginGroup("fea");
        DLGsettings->setValue("minElemementsPerLayer",DLGminElementsPerLayer);
        DLGsettings->setValue("maxElemementsPerLayer",DLGmaxElementsPerLayer);
        DLGsettings->setValue("numElementsInAir",DLGnumElementsInAir);
    DLGsettings->endGroup();
}
