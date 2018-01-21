#include "systemplotsuper.h"

SystemPlotSuper::SystemPlotSuper(QWidget *parent) :
    QWidget(parent)
{

}

SystemPlotSuper::~SystemPlotSuper()
{

}

void SystemPlotSuper::setLoadType(LoadControlType type)
{
    if (loadControlType != type)
    {
        loadControlType = type;
        this->refresh();
    }
}

void SystemPlotSuper::updatePiles(QMap<QString, double> &)
{
    this->refresh();
}

void SystemPlotSuper::updateSoil(QVector<double> &)
{
    this->refresh();
}

void SystemPlotSuper::updateLoad(double Px, double Py, double Moment)
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

void SystemPlotSuper::updateDisplacement(double ux, double uy)
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

void SystemPlotSuper::updateDispProfile(QVector<double> &profile)
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

void SystemPlotSuper::updatePileDeformation(QVector<double>, QVector<QVector<double> > &)
{
    this->refresh();
}

QList<QCPAbstractPlottable *> SystemPlotSuper::selectedPlottables()
{
    QList<QCPAbstractPlottable *> list;

    return list;
}
