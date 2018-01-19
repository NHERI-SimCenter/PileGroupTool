#include "systemplot.h"
#include "ui_systemplot.h"

SystemPlot::SystemPlot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemPlot)
{
    ui->setupUi(this);

    //
    // add legend
    //
    // now we move the legend from the inset layout of the axis rect into the main grid layout.
    // We create a sub layout so we can generate a small gap between the plot layout cell border
    // and the legend border:
    QCPLayoutGrid *subLayout = new QCPLayoutGrid;
    ui->plot->plotLayout()->addElement(1, 0, subLayout);
    subLayout->setMargins(QMargins(5, 0, 5, 5));
    subLayout->addElement(0, 0, ui->plot->legend);
    //
    // change the fill order of the legend, so it's filled left to right in columns:
    //ui->plot->legend->setFillOrder(QCPLegend::foColumnsFirst);
    ui->plot->legend->setRowSpacing(1);
    ui->plot->legend->setColumnSpacing(2);
    //ui->plot->legend->setFillOrder(QCPLayoutGrid::foColumnsFirst,true);

    // set legend's row stretch factor very small so it ends up with minimum height:
    ui->plot->plotLayout()->setRowStretchFactor(1, 0.001);

    //
    // default plot selection settings
    //
    activePileIdx = 0;
    activeLayerIdx = -1;
}

SystemPlot::~SystemPlot()
{
    delete ui;
}

void SystemPlot::refresh()
{

}

void SystemPlot::setLoadType(LoadControlType type)
{
    if (loadControlType != type)
    {
        loadControlType = type;
        this->refresh();
    }
}

void SystemPlot::updatePiles(QMap<QString, double> &)
{
    this->refresh();
}

void SystemPlot::updateLoad(double, double, double)
{
    this->refresh();
}

void SystemPlot::updateSoil(QVector<double> &)
{
    this->refresh();
}

void SystemPlot::updateDisplacement(double ux, double uy)
{
    this->refresh();
}

void SystemPlot::updateDispProfile(QVector<double> &)
{
    this->refresh();
}

void SystemPlot::updatePileDeformation(QVector<double>, QVector<QVector<double> > &)
{
    this->refresh();
}
