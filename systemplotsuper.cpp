/* ********************************************************************** *
 *
 * SystemPlotSuper is a virtual class used to setup and operate
 *    alternative implementations of SystemPlot.  At this point,
 *    the following derived classes provide the desired functionality:
 *
 *    1) SystemPlotQCP : QCustomPlot
 *    2) SystemPlotQwt : Qwt
 *
 * ********************************************************************** */

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

void SystemPlotSuper::updatePiles(QVector<PILE_INFO> &pileInfo)
{
    //
    // receive pile information
    //
    numPiles = pileInfo.size();
    if (numPiles > MAXPILES) { numPiles = MAXPILES; }

    for (int i=0; i<numPiles; i++)
    {
        L1              = pileInfo[i].L1;            // pile length above ground (all the same)
        L2[i]           = pileInfo[i].L2;            // embedded length of pile
        pileDiameter[i] = pileInfo[i].pileDiameter;  // pile diameter
        xOffset[i]      = pileInfo[i].xOffset;       // x-offset of pile
    }
    this->refresh();
}

void SystemPlotSuper::updateSoil(QVector<double> &layerDepth)
{
    int cnt = layerDepth.size();

    bool upToDate = true;

    if (cnt > 0) {
        if (depthOfLayer[0] != layerDepth[0] )
        {
            depthOfLayer[0] = layerDepth[0];
            upToDate = false;
        }
    } else {
        depthOfLayer[1] = 0.00;
        upToDate = false;
    }

    if (cnt > 1) {
        if (depthOfLayer[1] != layerDepth[1] )
        {
            depthOfLayer[1] = layerDepth[1];
            upToDate = false;
        }
    } else {
        depthOfLayer[1] = depthOfLayer[0];
        upToDate = false;
    }

    if (cnt > 2 ) {
        if (depthOfLayer[2] != layerDepth[2] )
        {
            depthOfLayer[2] = layerDepth[2];
            upToDate = false;
        }
    } else  {
        depthOfLayer[2] = depthOfLayer[1];
        upToDate = false;
    }

    if (cnt > 3) {
        if (depthOfLayer[3] != layerDepth[3] )
        {
            depthOfLayer[3] = layerDepth[3];
            upToDate = false;
        }
    } else {
        depthOfLayer[3] = depthOfLayer[2];
        upToDate = false;
    }
    if (!upToDate)  this->refresh();
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

void SystemPlotSuper::updatePileDeformation(QVector<double> &, QVector<QVector<double> > &)
{
    this->refresh();
}

QList<QCPAbstractPlottable *> SystemPlotSuper::selectedPlottables()
{
    QList<QCPAbstractPlottable *> list;

    return list;
}
