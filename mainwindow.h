#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// fixed parameters (limits for piles and soil layers)
#define MAXPILES 3
#define MAXLAYERS 3

// global constants
#define GAMMA_WATER 9.81

// functions
#define ABS(X) (X<0.0?-X:X)

// data types
typedef struct {
    int pileIdx;
    int nodeIdx;
    double x;
    double reductionFactorLeftMovement;
    double reductionFactorRightMovement;
} HEAD_NODE_TYPE;

#define SWAP(X,Y) {HEAD_NODE_TYPE tmp=Y; Y=X, Y=tmp; }

static QVector<QColor> LINE_COLOR({Qt::blue,Qt::red,Qt::green,Qt::cyan,Qt::magenta,Qt::yellow});

// forward declaration of classes
class soilLayer;
class QTableWidgetItem;
class QSettings;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void doAnalysis(void);
    void fetchSettings();
    void updateUI();
    void updateSystemPlot();

private slots:
    // soil parameter values entered/changed

    // program controls
    //void on_analyzeButton_clicked();

    // menu actions
    void on_actionExit_triggered();
    void on_action_Open_triggered();
    void on_actionExport_to_OpenSees_triggered();
    void on_actionReset_triggered();
    void on_actionFEA_parameters_triggered();
    void on_actionLicense_Information_triggered();
    void on_action_About_triggered();
    void on_actionPreferences_triggered();

    // check boxes
    void on_chkBox_assume_rigid_cap_clicked(bool checked);
    void on_chkBox_include_toe_resistance_clicked(bool checked);

    // geometry parameters entered/changed
    void on_displacementSlider_valueChanged(int value);
    void on_pileDiameter_valueChanged(double arg1);
    void on_embeddedLength_valueChanged(double arg1);
    void on_freeLength_valueChanged(double arg1);
    void on_Emodulus_valueChanged(double arg1);
    void on_groundWaterTable_valueChanged(double arg1);
    void on_xOffset_valueChanged(double arg1);
    void on_pileIndex_valueChanged(int arg1);
    void on_btn_deletePile_clicked();
    void on_btn_newPile_clicked();

    // material table slots
    void on_appliedForce_valueChanged(double arg1);
    void on_appliedForce_editingFinished();
    void on_updateInfo(QTableWidgetItem *);

private:
    Q_OBJECT
    Ui::MainWindow *ui;

    // get data
    double P;  // lateral force on pile
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
    void reDrawTable();

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

    // system conforming settings and parameters
    QSettings *settings = NULL;

    // viewer settings
    bool showDisplacements;
    bool showMoments;
    bool showShear;
    bool showStress;
    bool showPultimate;
    bool showY50;

    // meshing parameters
    int minElementsPerLayer = 15;
    int maxElementsPerLayer = 40;
    int numElementsInAir    =  4;

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

};

#endif // MAINWINDOW_H
