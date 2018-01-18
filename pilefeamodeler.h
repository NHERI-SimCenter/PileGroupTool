#ifndef PILEFEAMODELER_H
#define PILEFEAMODELER_H

#include <QVector>
#include <QMap>

#include "pilegrouptool_parameters.h"
#include "soilmat.h"

class PileFEAmodeler
{
public:
    PileFEAmodeler();

    void updatePiles(QMap<QString, double> &);
    void updateLoad(double, double, double);
    void updateSoil(QVector<soilLayer> &);
    void updateDisplacement(double);
    void updateDispProfile(QVector<double> &);
    void setAnalysisType(QString);
    void doAnalysis();

    int  getExitStatus();

protected:

    // load control
    QString loadControlType;

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
    bool useToeResistance    = false;
    int  puSwitch;
    int  kSwitch;
    int  gwtSwitch;

    // load parameter
    double displacementRatio;

    // soil layers and related methods
    QVector<soilLayer> mSoilLayers;

    void setupLayers();

    // temporary variables
    double gamma;
    double gammaWet;
    double gammaSaturated;
    double phi;
    double gSoil;
    double totalStress;
    double effectiveStress;
    double porePressure;
    double overburdonStress;
    double groundWaterHead;

    double zCoord = 0.0;  // z-coordinate of point.  Negative if below the surface
    double eleSize;       // effective element length for p-y and t-z springs
    double sigV;          // effective stress at p-y and t-z springs
    double pult, y50;     // lateral resistance
    double tult, z50;     // shaft friction
    double qult, z50q;    // toe resistance

    // setup switch
    bool inSetupState = true;

    // viewer settings
    bool showDisplacements;
    bool showMoments;
    bool showShear;
    bool showStress;
    bool showPultimate;
    bool showY50;

    // meshing parameters
    int minElementsPerLayer = MIN_ELEMENTS_PER_LAYER;
    int maxElementsPerLayer = MAX_ELEMENTS_PER_LAYER;
    int numElementsInAir    = NUM_ELEMENTS_IN_AIR;

    double L1;                      // pile length above ground (all the same)
    double L2[MAXPILES];            // embedded length of pile
    double pileDiameter[MAXPILES];  // pile diameter
    double E[MAXPILES];             // pile modulus of elasticity
    double xOffset[MAXPILES];       // x-offset of pile

    int numNodePiles;
    int numNodePile[MAXPILES];
    int maxLayers[MAXPILES];
    int nodeIDoffset[MAXPILES];
    int elemIDoffset[MAXPILES];

    // pile head parameters
    double EI = 1.;
    double EA = 1.;
    double GJ = 1.0e12;

    // others
    QVector<HEAD_NODE_TYPE> headNodeList = QVector<HEAD_NODE_TYPE>(MAXPILES, {-1,-1,0.0, 1.0, 1.0});
};

#endif // PILEFEAMODELER_H
