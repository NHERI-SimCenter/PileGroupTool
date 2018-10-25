#ifndef PILEFEAMODELER_H
#define PILEFEAMODELER_H

//#include <QVector>
#include <QMap>
#include <QFile>

#include "pilegrouptool_parameters.h"
#include "soilmat.h"

#define CHECK_STATE(X)   modelState.value(X)
#define ENABLE_STATE(X)  modelState[X]=true
#define DISABLE_STATE(X) modelState[X]=false

enum class PilePlotData {
    LateralDisplacement,
    AxialDisplacement,
    PileMoment,
    PileShear,
    PileAxialForce,
    SoilStress,
    Pultimate,
    Y50,
    Tultimate,
    Z50
};

enum class AnalysisState {
    solutionAvailable,
    solutionValid,
    dataExtracted,
    meshValid,
    loadValid,
    analysisValid
};

class Domain;
class StaticAnalysis;

class PileFEAmodeler
{
public:
    PileFEAmodeler();
    ~PileFEAmodeler();

    class SoilMotionData
    {
    public:
        SoilMotionData() { delta0=0.0; delta1=0.0;zmax=999999.0;};
        SoilMotionData(double d0, double d1, double zm) { delta0=d0; delta1=d1;zmax=zm;};

        double  delta0;
        double  delta1;
        double  zmax;
    };

    class SoilNodeData
    {
    public:
        SoilNodeData() {ID=-1; depth=0;};
        SoilNodeData(int id, double z) {ID=id; depth=z;};

        int     ID;
        double  depth;
    };

    void updatePiles(QVector<PILE_INFO> &);
    void updateSwitches(bool useToe, bool assumeRigidHead);
    void setLoadType(LoadControlType);
    void updateLoad(double, double, double);
    void updateSoil(QVector<soilLayer> &);
    void updateGWtable(double );
    void updateDisplacement(double ux=0.0, double uy=0.0);
    void updateDispProfile(QVector<double> &);
    void setAnalysisType(QString);
    void setDefaultParameters(void);
    bool doAnalysis();

    void writeFEMinput(QString filename);
    void dumpDomain(QString filename);

    int  getExitStatus();

    void buildMesh();
    void buildLoad();
    void buildAnalysis();

    void updateMotionData(void);
    double shift(double z);

    QList<QVector<QVector<double> *> *> getLateralDisplacements();
    QList<QVector<QVector<double> *> *> getAxialDisplacements();
    QList<QVector<QVector<double> *> *> getMoment();
    QList<QVector<QVector<double> *> *> getShear();
    QList<QVector<QVector<double> *> *> getForce();
    QList<QVector<QVector<double> *> *> getStress();
    QList<QVector<QVector<double> *> *> getPult();
    QList<QVector<QVector<double> *> *> getY50();
    QList<QVector<QVector<double> *> *> getTult();
    QList<QVector<QVector<double> *> *> getZ50();

private:
    int extractPlotData();
    void clearPlotBuffers();

protected:
    // load control
    LoadControlType loadControlType;

    QMap<AnalysisState, bool> modelState;

    double P;     // lateral force on pile cap
    double PV;    // vertical force on pile cap
    double PMom;  // applied moment on pile cap

    double excentricity;  // horizontal offset of the first pile from the center of the pile cap

    double HDisp; // prescribed horizontal displacement
    double VDisp; // prescriber vertical displacement

    double surfaceDisp;    // prescribed soil surface displacement
    double percentage12;   // percentage of surface displacement at 1st layer interface
    double percentage23;   // percentage of surface displacement at 2nd layer interface
    double percentageBase; // percentage of surface displacement at base of soil column

    QVector<double> soilMotion = QVector<double>(MAXLAYERS+1, 0.0);
    QVector<SoilMotionData> motionData = QVector<SoilMotionData>(MAXLAYERS);

    // get parameters
    double gwtDepth;  // depth of ground water table below the surface
    int    numPileElements;
    int    numPiles;

    // states
    bool assumeRigidPileHeadConnection = false;
    bool useToeResistance    = true;
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

    // meshing parameters
    int minElementsPerLayer = MIN_ELEMENTS_PER_LAYER;
    int maxElementsPerLayer = MAX_ELEMENTS_PER_LAYER;
    int numElementsInAir    = NUM_ELEMENTS_IN_AIR;

    PILE_FEA_INFO pileInfo[MAXPILES];

    int numNodePiles;

    // pile head parameters
    double EI = 1.;
    double EA = 1.;
    double GJ = 1.0e12;

    // others
    QVector<HEAD_NODE_TYPE> headNodeList = QVector<HEAD_NODE_TYPE>(MAXPILES, {-1,-1,0.0, 1.0, 1.0});
    QList<CAP_NODE_TYPE>    capNodeList;

    Domain *theDomain;
    StaticAnalysis *theAnalysis = nullptr;

    int numLoadedNode;
    QVector<double> depthOfLayer = QVector<double>(4, 0.0);

    // data containers
    QVector<QVector<double> *> locList;
    QVector<QVector<double> *> lateralDispList;
    QVector<QVector<double> *> axialDispList;
    QVector<QVector<double> *> MomentList;
    QVector<QVector<double> *> ShearList;
    QVector<QVector<double> *> AxialList;
    QVector<QVector<double> *> StressList;

    QVector<QVector<double> *> pultList;
    QVector<QVector<double> *> y50List;
    QVector<QVector<double> *> tultList;
    QVector<QVector<double> *> z50List;

    QList<SoilNodeData> soilNodes;

    //
    // state switches
    bool    dumpFEMinput;
    QFile  *FEMfile = nullptr;
};

#endif // PILEFEAMODELER_H
