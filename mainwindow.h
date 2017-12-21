#ifndef MAINWINDOW_H
#define MAINWINDOW_H


class QNetworkAccessManager;

#include <QMainWindow>
#include <qcustomplot.h>
#include <QtNetwork/QNetworkReply>


// fixed parameters (limits for piles and soil layers)
#define MAXPILES 3
#define MAXLAYERS 3

// Meshing parameters
#define MIN_ELEMENTS_PER_LAYER   15
#define MAX_ELEMENTS_PER_LAYER   40
#define NUM_ELEMENTS_IN_AIR       4

// #define MAX_FORCE 5000.0
#define MAX_FORCE  10000.0
#define MAX_MOMENT 25000.0
#define MAX_DISP   2.0

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
static QVector<QColor> BRUSH_COLOR({
                                       QColor(255, 127, 0, 127),
                                       QColor(191,  95, 0, 127),
                                       QColor(127,  63, 0, 127),
                                       QColor(127, 255, 0, 127),
                                       QColor( 95, 191, 0, 127),
                                       QColor( 63, 127, 0, 127),
                                       QColor(127, 255, 255, 255),
                                       QColor( 95, 191, 255, 255),
                                       QColor( 63, 127, 255, 255),
                                       QColor(127, 255, 255, 127),
                                       QColor( 95, 191, 255, 127),
                                       QColor( 63, 127, 255, 127)
                                    });
#define GROUND_WATER_BLUE QColor(127,127,255,192)

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
    void setActiveLayer(int);
    void updateLayerState();
    int  findActiveLayer();
    int  adjustLayersToPiles();
    void plotResults(QCustomPlot *, QVector<double> z, QVector<double> xOffset,
                     QVector<QVector<double> >, QVector<QVector<double> >);

private slots:
    // soil parameter values entered/changed

    // program controls
    //void on_analyzeButton_clicked();

    // menu actions
    void on_actionExit_triggered();
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_action_Open_triggered();
    void on_actionExport_to_OpenSees_triggered();
    void on_actionReset_triggered();
    void on_actionFEA_parameters_triggered();

    void on_actionLicense_Information_triggered();
    void on_actionLicense_triggered();
    void on_actionVersion_triggered();
    void on_actionProvide_Feedback_triggered();

  //  void on_actionLicense_triggered();
  //  void on_actionVersion_triggered();
  //  void on_actionProvide_Feedback_triggered();

    void on_action_About_triggered();
    void on_actionPreferences_triggered();

    // check boxes
    void on_chkBox_assume_rigid_cap_clicked(bool checked);
    void on_chkBox_include_toe_resistance_clicked(bool checked);

    // geometry parameters entered/changed
    void on_horizontalForceSlider_valueChanged(int value);
    void on_verticalForceSlider_valueChanged(int value);
    void on_momentSlider_valueChanged(int value);

    void on_pileDiameter_valueChanged(double arg1);
    void on_embeddedLength_valueChanged(double arg1);
    void on_freeLength_valueChanged(double arg1);
    void on_Emodulus_valueChanged(double arg1);
    void on_groundWaterTable_valueChanged(double arg1);
    void on_xOffset_valueChanged(double arg1);
    void on_pileIndex_valueChanged(int arg1);
    void on_btn_deletePile_clicked();
    void on_btn_newPile_clicked();
    void on_systemPlot_selectionChangedByUser();

    // material table slots
    void on_appliedHorizontalForce_editingFinished();
    void on_appliedVerticalForce_editingFinished();
    void on_appliedMoment_editingFinished();

    // layer selection slots
    void on_chkBox_layer1_clicked();
    void on_chkBox_layer2_clicked();
    void on_chkBox_layer3_clicked();
    void on_layerSelectedInSystemPlot(bool selected);

    // soil property slots
    void on_layerThickness_valueChanged(double arg1);
    void on_layerDryWeight_valueChanged(double arg1);
    void on_layerSaturatedWeight_valueChanged(double arg1);
    void on_layerFrictionAngle_valueChanged(double arg1);
    void on_layerShearModulus_valueChanged(double arg1);

    void on_properties_currentChanged(int index);

    // Frank's network counter
    void replyFinished(QNetworkReply*);

    void on_forceTypeSelector_activated(int index);
    void on_pushoverDisplacement_editingFinished();
    void on_pulloutDisplacement_editingFinished();
    void on_pushoverDisplacementSlider_valueChanged(int value);
    void on_pulloutDisplacementSlider_valueChanged(int value);
    void on_surfaceDisplacement_editingFinished();
    void on_surfaceDisplacementSlider_valueChanged(int value);
    void on_Interface12_editingFinished();
    void on_Interface12Slider_valueChanged(int value);
    void on_Interface23_editingFinished();
    void on_Interface23Slider_valueChanged(int value);
    void on_BaseDisplacement_editingFinished();
    void on_BaseDisplacementSlider_valueChanged(int value);

private:
    Q_OBJECT
    Ui::MainWindow *ui;

    void updateSystemPlot();
    void refreshUI();
    bool ReadFile(QString );
    bool WriteFile(QString );

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
    int activePileIdx;
    int activeLayerIdx;

     QNetworkAccessManager *manager;

};

#endif // MAINWINDOW_H
