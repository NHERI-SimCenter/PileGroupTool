#include "systemplotqwt.h"
#include "ui_systemplotqwt.h"

SystemPlotQwt::SystemPlotQwt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemPlotQwt)
{
    ui->setupUi(this);
}

SystemPlotQwt::~SystemPlotQwt()
{
    delete ui;
}
