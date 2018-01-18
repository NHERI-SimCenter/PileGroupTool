#include "systemplot.h"
#include "ui_systemplot.h"

SystemPlot::SystemPlot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemPlot)
{
    ui->setupUi(this);
}

SystemPlot::~SystemPlot()
{
    delete ui;
}
