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
void SystemPlotQwt::refresh()
{

}

void SystemPlotQwt::setLoadType(LoadControlType type)
{
    if (loadControlType != type)
    {
        loadControlType = type;
        this->refresh();
    }
}

void SystemPlotQwt::updatePiles(QMap<QString, double> &)
{
    this->refresh();
}

void SystemPlotQwt::updateSoil(QVector<double> &)
{
    this->refresh();
}

void SystemPlotQwt::updateLoad(double Px, double Py, double Moment)
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

void SystemPlotQwt::updateDisplacement(double ux, double uy)
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

void SystemPlotQwt::updateDispProfile(QVector<double> &profile)
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

void SystemPlotQwt::updatePileDeformation(QVector<double>, QVector<QVector<double> > &)
{
    this->refresh();
}
