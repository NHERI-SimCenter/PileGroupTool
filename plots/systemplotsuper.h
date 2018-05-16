#ifndef SYSTEMPLOTSUPER_H
#define SYSTEMPLOTSUPER_H

#include <QWidget>
#include <QVector>
#include "pilegrouptool_parameters.h"
#include "qcp/qcustomplot.h"

#define PLOT_DEFORMED
//#undef PLOT_DEFORMED

namespace Ui {
class SystemPlotSuper;
}

class SystemPlotSuper : public QWidget
{
public:
    explicit SystemPlotSuper(QWidget *parent = 0);
    ~SystemPlotSuper();

    struct SOIL_MOTION_DATA {
        double  delta0;
        double  delta1;
        double  zmax;
    };

    virtual void refresh() = 0;
    virtual void setLoadType(LoadControlType);
    virtual void updatePiles(QVector<PILE_INFO> &);
    virtual void updateLoad(double, double, double);
    virtual void updateSoil(QVector<double> &);
    virtual void updateGWtable(double);
    virtual void updateDisplacement(double ux=0.0, double uy=0.0);
    virtual void updateDispProfile(double surfaceDisp, double percentage12, double percentage23, double percentageBase);

    virtual void updatePileDeformation(QVector<QVector<double> *> &, QVector<QVector<double> *> &, QVector<QVector<double> *> &);
    virtual void updateMotionData(void);
    virtual double shift(double z);

    virtual void setActivePile(int index) {activePileIdx = index;}
    virtual int  getActivePileIndex() {return activePileIdx;}
    virtual void setActiveLayer(int index) {activeLayerIdx = index;}
    virtual int  getActiveLayerIndex() {return activeLayerIdx;}

    virtual void setSystemStable(bool status) { mIsStable = status; };

    virtual QList<QCPAbstractPlottable *> selectedPlottables();

private:

protected:
    LoadControlType loadControlType;

    double P    = 0.0;  // lateral force on pile cap
    double PV   = 0.0;  // vertical force on pile cap
    double PMom = 0.0;  // applied moment on pile cap

    double HDisp = 0.0; // prescribed horizontal displacement
    double VDisp = 0.0; // prescriber vertical displacement

    double surfaceDisp    = 0.0; // prescribed soil surface displacement
    double percentage12   = 1.0; // percentage of surface displacement at 1st layer interface
    double percentage23   = 0.0; // percentage of surface displacement at 2nd layer interface
    double percentageBase = 0.0; // percentage of surface displacement at base of soil column

    QVector<double> soilMotion = QVector<double>(MAXLAYERS+1, 0.0);
    QVector<SOIL_MOTION_DATA> motionData = QVector<SOIL_MOTION_DATA>(MAXLAYERS);

    // get parameters
    double gwtDepth;  // depth of ground water table below the surface

    int    numPileElements;
    int    numPiles;

    // states
    bool assumeRigidPileHeadConnection = false;

    // meshing parameters
    int minElementsPerLayer = MIN_ELEMENTS_PER_LAYER;
    int maxElementsPerLayer = MAX_ELEMENTS_PER_LAYER;
    int numElementsInAir    = NUM_ELEMENTS_IN_AIR;

    double L1;                      // pile length above ground (all the same)
    double L2[MAXPILES];            // embedded length of pile
    double pileDiameter[MAXPILES];  // pile diameter
    double xOffset[MAXPILES];       // x-offset of pile

    int numNodePiles;
    int numNodePile[MAXPILES];
    int maxLayers[MAXPILES];
    int nodeIDoffset[MAXPILES];
    int elemIDoffset[MAXPILES];

    // others
    QVector<HEAD_NODE_TYPE> headNodeList = QVector<HEAD_NODE_TYPE>(MAXPILES, {-1,-1,0.0, 1.0, 1.0});

    int numLoadedNode;
    QVector<double> depthOfLayer = QVector<double>(4, 0.0);

    // selection tracking
    int activePileIdx = 0;
    int activeLayerIdx = -1;

    // deformed pile information
    QVector<QVector<double> *> m_pos;
    QVector<QVector<double> *> m_dispU;
    QVector<QVector<double> *> m_dispV;

    // system status info
    bool    mIsStable = true;
};

#endif // SYSTEMPLOTSUPER_H
