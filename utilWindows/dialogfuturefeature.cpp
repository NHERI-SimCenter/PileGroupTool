#include "dialogfuturefeature.h"
#include "ui_dialogfuturefeature.h"

DialogFutureFeature::DialogFutureFeature(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFutureFeature)
{
    ui->setupUi(this);
}

DialogFutureFeature::~DialogFutureFeature()
{
    delete ui;
}

void DialogFutureFeature::on_buttonBox_accepted()
{
    this->close();
}
