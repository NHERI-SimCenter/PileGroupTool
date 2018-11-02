#ifndef MAINWINDOW_H
#define MAINWINDOW_H


class QNetworkAccessManager;

#include <QMainWindow>
#include <QtNetwork/QNetworkReply>
#include "pilegrouptool_parameters.h"

// forward declaration of classes
class soilLayer;
class QTableWidgetItem;
class QSettings;
class PileFEAmodeler;
class SystemPlotSuper;
class ResultPlotSuper;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{

public:
    explicit MainWindow(bool graphicsModeQCP = false, QWidget *parent = 0);
    ~MainWindow();

    void doAnalysis(void);
    void fetchSettings();
    void updateUI();
    void setActiveLayer(int);
    void updateLayerState();
    int  findActiveLayer();
    int  adjustLayersToPiles();
    void updateResultPlots();
    void updateSoilResultPlots(QVector<double> &);
    void refreshResultPlots();



private slots:
    //
    // program controls
    //

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
    void on_actionProvide_Feedback_triggered();

    void on_action_About_triggered();
    void on_actionQuick_Tips_triggered();
    void on_actionPreferences_triggered();

    // check boxes
    void on_chkBox_assume_rigid_cap_clicked(bool checked);
    void on_chkBox_include_toe_resistance_clicked(bool checked);

    // geometry parameters entered/changed
    void on_pileDiameter_valueChanged(double arg1);
    void on_embeddedLength_valueChanged(double arg1);
    void on_freeLength_valueChanged(double arg1);
    void on_Emodulus_valueChanged(double arg1);
    void on_groundWaterTable_valueChanged(double arg1);
    void on_xOffset_valueChanged(double arg1);
    void on_pileIndex_valueChanged(int arg1);

    void on_btn_deletePile_clicked();
    void on_btn_newPile_clicked();

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

    // load control slots
    void on_forceTypeSelector_activated(int index);

    void on_HForceCtrl_valueChanged(int value);
    void on_VForceCtrl_valueChanged(int value);
    void on_MomentCtrl_valueChanged(int value);

    void on_PushOverCtrl_valueChanged(int value);
    void on_PullOutCtrl_valueChanged(int value);

    void on_SurfaceDisplacementCtrl_valueChanged(int value);
    void on_Interface12Ctrl_valueChanged(int value);
    void on_Interface23Ctrl_valueChanged(int value);
    void on_BaseDisplacementCtrl_valueChanged(int value);

    // response to signals from systemPlot
    void onSystemPlot_pileSelected(int );
    void onSystemPlot_soilLayerSelected(int );
    void onSystemPlot_groundWaterSelected();

private:
    Q_OBJECT
    Ui::MainWindow *ui;

    SystemPlotSuper *systemPlot = nullptr;
    PileFEAmodeler *pileFEAmodel = nullptr;

    void updateSystemPlot();
    void refreshUI();
    bool ReadFile(QString );
    bool WriteFile(QString );

    // load control
    LoadControlType loadControlType;

    // prescribed loads at the pile head
    double P;     // lateral force on pile cap
    double PV;    // vertical force on pile cap
    double PMom;  // applied moment on pile cap

    // pile head displacement (push-over analysis)
    double HDisp; // prescribed horizontal displacement
    double VDisp; // prescriber vertical displacement

    // soil motion profile
    double surfaceDisp;    // prescribed soil surface displacement
    double percentage12;   // percentage of surface displacement at 1st layer interface
    double percentage23;   // percentage of surface displacement at 2nd layer interface
    double percentageBase; // percentage of surface displacement at base of soil column

    // ground water table
    double gwtDepth;  // depth of ground water table below the surface

    // general parameters
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

    // system conforming settings and parameters
    QSettings *settings = nullptr;

    // general settings
    QString useGraphicsLib;  // "QCP" or "QwtAll" or "QwtSystem" or "QwtResults"
    QString useFEAnalyzer;   // "OpenSeesInt" is currently the only implemented option

    // viewer settings
    bool showDisplacements;
    bool showPullOut;
    bool showMoments;
    bool showShear;
    bool showAxial;
    bool showStress;
    bool showPultimate;
    bool showY50;
    bool showTultimate;
    bool showZ50;

    // meshing parameters
    int minElementsPerLayer = MIN_ELEMENTS_PER_LAYER;
    int maxElementsPerLayer = MAX_ELEMENTS_PER_LAYER;
    int numElementsInAir    = NUM_ELEMENTS_IN_AIR;

    double L1;                      // pile length above ground (all the same)
    double L2[MAXPILES];            // embedded length of pile
    double pileDiameter[MAXPILES];  // pile diameter
    double E[MAXPILES];             // pile modulus of elasticity
    double xOffset[MAXPILES];       // x-offset of pile

    int activePileIdx;
    int activeLayerIdx;

     QNetworkAccessManager *manager;

     // plot widgets
     ResultPlotSuper *displPlot;
     ResultPlotSuper *pullOutPlot;
     ResultPlotSuper *momentPlot;
     ResultPlotSuper *shearPlot;
     ResultPlotSuper *axialPlot;
     ResultPlotSuper *stressPlot;
     ResultPlotSuper *pultPlot;
     ResultPlotSuper *y50Plot;
     ResultPlotSuper *tultPlot;
     ResultPlotSuper *z50Plot;

};

#endif // MAINWINDOW_H
