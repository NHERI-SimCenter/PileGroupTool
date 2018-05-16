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
    SOIL_MOTION_DATA data;
    data.delta0 = 0.0;
    data.delta1 = 0.0;
    data.zmax   = 0.0;

    motionData = QVector<SOIL_MOTION_DATA>(MAXLAYERS, data);

    m_pos.clear();
    m_dispU.clear();
    m_dispV.clear();

    mIsStable = false;
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

void SystemPlotSuper::updateGWtable(double depth)
{
    if (gwtDepth == depth) return;

    gwtDepth = depth;
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

void SystemPlotSuper::updateDispProfile(double new_surfaceDisp,
                                        double new_percentage12,
                                        double new_percentage23,
                                        double new_percentageBase)
{
    bool upToDate = true;

    if (surfaceDisp != new_surfaceDisp)
    {
        surfaceDisp = new_surfaceDisp;
        upToDate = false;
    }
    if (percentage12 != new_percentage12)
    {
        percentage12 = new_percentage12;
        upToDate = false;
    }
    if (percentage23 != new_percentage23)
    {
        percentage23 = new_percentage23;
        upToDate = false;
    }
    if (percentageBase != new_percentageBase)
    {
        percentageBase = new_percentageBase;
        upToDate = false;
    }
    if (!upToDate)
    {
        soilMotion[0] = surfaceDisp;
        soilMotion[1] = surfaceDisp*percentage12;
        soilMotion[2] = surfaceDisp*percentage23;
        soilMotion[3] = surfaceDisp*percentageBase;

        this->updateMotionData();
        this->refresh();
    }
}

void SystemPlotSuper::updatePileDeformation(QVector<QVector<double> *> &pos,
                                            QVector<QVector<double> *> &dispU,
                                            QVector<QVector<double> *> &dispV)
{
    if (pos.size() != numPiles || dispU.size() != numPiles || dispV.size() != numPiles )
    {
        qWarning() << "incompatible deformation data in SystemPlot -- ignoring data";

        QVector<QVector<double> *> dummy;
        dummy.clear();

        m_pos   = dummy;
        m_dispU = dummy;
        m_dispV = dummy;

        mIsStable = false;

        return;
    }

    m_pos   = pos;
    m_dispU = dispU;
    m_dispV = dispV;

    mIsStable = true;

    this->refresh();
}

double SystemPlotSuper::shift(double z)
{
    double s = soilMotion.last();

    QVectorIterator<SOIL_MOTION_DATA> itr(motionData);

    while (itr.hasNext())
    {
        SOIL_MOTION_DATA info = itr.next();

        if (info.zmax >= z)
        {
            s = info.delta0 + z * info.delta1;
            break;
        }
    }

    return s;
}

void SystemPlotSuper::updateMotionData(void)
{
    SOIL_MOTION_DATA info;

    for (int i=0; i<MAXLAYERS; i++)
    {
        double si = soilMotion[i];
        double sj = soilMotion[i+1];
        double zi = depthOfLayer[i];
        double zj = depthOfLayer[i+1];

        info.delta1 = (sj - si)/(zj - zi);
        info.delta0 = si - zi * info.delta1;
        info.zmax   = zj;

        motionData[i] = info;
    }
}

QList<QCPAbstractPlottable *> SystemPlotSuper::selectedPlottables()
{
    QList<QCPAbstractPlottable *> list;

    return list;
}
