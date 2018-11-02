#include "sctrinputgroup.h"
#include "ui_sctrinputgroup.h"

SCtrInputGroup::SCtrInputGroup(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SCtrInputGroup)
{
    ui->setupUi(this);
    this->setRange(-100,100);

    connect(ui->slider, SIGNAL(valueChanged(int)), ui->spinBox,SLOT(setValue(int)));
    connect(ui->spinBox,SIGNAL(valueChanged(int)), ui->slider, SLOT(setValue(int)));
    connect(ui->slider, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));
}

SCtrInputGroup::~SCtrInputGroup()
{
    delete ui;
}

void SCtrInputGroup::setValue(int val)
{
    ui->slider->setValue(val);
    ui->spinBox->setValue(val);
}

void SCtrInputGroup::setMinimum(int val)
{
    ui->slider->setMinimum(val);
    ui->spinBox->setMinimum(val);
}

void SCtrInputGroup::setMaximum(int val)
{
    ui->slider->setMaximum(val);
    ui->spinBox->setMaximum(val);
}

void SCtrInputGroup::setTickInterval(int val)
{
    ui->slider->setTickInterval(val);
}

void SCtrInputGroup::setSingleStep(int val)
{
    ui->spinBox->setSingleStep(val);
}

void SCtrInputGroup::setRange(int min, int max)
{
    ui->slider->setRange(min, max);
    ui->spinBox->setRange(min,max);
}
