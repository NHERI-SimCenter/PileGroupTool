#ifndef PILEFEAMODELER_H
#define PILEFEAMODELER_H

#include <QVector>
#include <QMap>

#include "pilegrouptool_parameters.h"
#include "soilmat.h"

class Domain;
class StaticAnalysis;

class PileFEAmodeler
{
public:
    PileFEAmodeler();

    void updatePiles(QVector<PILE_INFO> &);
    void setLoadType(LoadControlType);
    void updateLoad(double, double, double);
    void updateSoil(QVector<soilLayer> &);
    void updateGWtable(double );
    void updateDisplacement(double ux=0.0, double uy=0.0);
    void updateDispProfile(QVector<double> &);
    void setAnalysisType(QString);
    void setDefaultParameters(void);
    void doAnalysis();
    void extractPlotData();

    int  getExitStatus();

    void buildMesh();
    void buildLoad();
    void buildAnalysis();

    QList<QVector<double> > *getDisplacements(int pile=0, int dir=0);
    QList<QVector<double> > *getMoment(int pile=0);
    QList<QVector<double> > *getShear(int pile=0);
    QList<QVector<double> > *getForce(int pile=0);
    QList<QVector<double> > *getPult(int pile=0);
    QList<QVector<double> > *getY50(int pile=0);

protected:

    // load control
    LoadControlType loadControlType;

    QMap<QString, bool> modelState;

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

    // soil layers and related methods
    QVector<soilLayer> mSoilLayers;

    void setupLayers();

    // temporary variables
    double gSoil;

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
    bool showPullOut;
    bool showMoments;
    bool showShear;
    bool showAxial;
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

    Domain *theDomain;
    StaticAnalysis *theAnalysis = NULL;

    int numLoadedNode;
    QVector<double> depthOfLayer = QVector<double>(4, 0.0);
};

#endif // PILEFEAMODELER_H
