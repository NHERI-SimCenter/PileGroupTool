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

void SystemPlot::updateSoil(QVector<double> &)
{
    this->refresh();
}

void SystemPlot::updateLoad(double Px, double Py, double Moment)
{
    bool upToDate = true;

    if (P != Px)
    {
        P  = Px;
        upToDate = false;
    }
    if (PV != Py)
    {
        PV = Py;
        upToDate = false;
    }
    if (PMom != Moment)
    {
        PMom = Moment;
        upToDate = false;
    }
    if (!upToDate)  this->refresh();
}

void SystemPlot::updateDisplacement(double ux, double uy)
{
    bool upToDate = true;

    if (HDisp != ux)
    {
        HDisp = ux;
        upToDate = false;
    }
    if (VDisp != uy)
    {
        VDisp = uy;
        upToDate = false;
    }
    if (!upToDate)  this->refresh();
}

void SystemPlot::updateDispProfile(QVector<double> &profile)
{
    bool upToDate = true;

    if (surfaceDisp != profile[0])
    {
        surfaceDisp = profile[0];
        upToDate = false;
    }
    if (percentage12 != profile[1])
    {
        percentage12 = profile[1];
        upToDate = false;
    }
    if (percentage23 != profile[2])
    {
        percentage23 = profile[2];
        upToDate = false;
    }
    if (percentageBase != profile[3])
    {
        percentageBase = profile[3];
        upToDate = false;
    }
    if (!upToDate)  this->refresh();
}

void SystemPlot::updatePileDeformation(QVector<double>, QVector<QVector<double> > &)
{
    this->refresh();
}
