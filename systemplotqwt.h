#ifndef SYSTEMPLOTQWT_H
#define SYSTEMPLOTQWT_H

#include <QWidget>
#include "pilegrouptool_parameters.h"

namespace Ui {
class SystemPlotQwt;
}

class SystemPlotQwt : public QWidget
{
    Q_OBJECT

public:
    explicit SystemPlotQwt(QWidget *parent = 0);
    ~SystemPlotQwt();

    void refresh();
    void setLoadType(LoadControlType);
    void updatePiles(QMap<QString, double> &);
    void updateLoad(double, double, double);
    void updateSoil(QVector<double> &);
    void updateDisplacement(double ux=0.0, double uy=0.0);
    void updateDispProfile(QVector<double> &);
    void updatePileDeformation(QVector<double>, QVector<QVector<double> > &);

    void setActivePile(int index) {activePileIdx = index;}
    int  getActivePileIndex() {return activePileIdx;}
    void setActiveLayer(int index) {activeLayerIdx = index;}
    int  getActiveLayerIndex() {return activeLayerIdx;}

private:
    Ui::SystemPlotQwt *ui;

protected:
    LoadControlType loadControlType;

    double P;     // lateral force on pile cap
    double PV;    // vertical force on pile cap
    double PMom;  // applied moment on pile cap

    double HDisp; // prescribed horizontal displacement
    double VDisp; // prescriber vertical displacement

    double surfaceDisp;    // prescribed soil surface displacement
    double percentage12;   // percentage of surface displacement at 1st layer interface
    double percentage23;   // percentage of surface displacement at 2nd layer interface
    double percentageBase; // percentage of surface displacement at base of soil column

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
};

#endif // SYSTEMPLOTQWT_H
