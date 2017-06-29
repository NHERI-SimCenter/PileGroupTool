#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void doAnalysis(void);

private slots:
    // soil parameter values entered/changed
    void on_gammaValue_valueChanged(double arg1);
    void on_phiValue_valueChanged(double arg1);
    void on_gSoilValue_valueChanged(double arg1);
    void on_gammaValue_editingFinished();

    // program controls
    //void on_analyzeButton_clicked();

    // menu actions
    void on_actionExit_triggered();

    // check boxes
    void on_chkBox_assume_rigid_cap_clicked(bool checked);
    void on_chkBox_include_toe_resistance_clicked(bool checked);

    // geometry parameters entered/changed
    void on_displacementSlider_sliderMoved(int position);
    void on_pileDiameter_valueChanged(double arg1);
    void on_embeddedLength_valueChanged(double arg1);
    void on_freeLength_valueChanged(double arg1);
    void on_Emodulus_valueChanged(double arg1);
    void on_groundWaterTable_valueChanged(double arg1);

private:
    Q_OBJECT
    Ui::MainWindow *ui;

    // get data
    double P;
    double L1;
    double L2;
    double D;
    double E;
    int numEle;
    double gamma;
    double phi;
    double gSoil;
    int puSwitch;
    int kSwitch;
    int gwtSwitch;
    double gwtDepth;

    // states
    bool assumeRigidPileHead = false;
    bool useToeResistance    = false;

    // load parameter
    double displacementRatio;

};

#endif // MAINWINDOW_H
