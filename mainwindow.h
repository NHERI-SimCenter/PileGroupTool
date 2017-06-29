#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// forward declaration of classes
class soilLayer;
class QTableWidgetItem;

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
    // parameter values entered/changed
    void on_puValue_activated(const QString &arg1);
    void on_fixedHead_clicked(bool checked);
    void on_pinnedHead_clicked(bool checked);
    void on_pValue_valueChanged(double arg1);
    void on_l1Value_valueChanged(double arg1);
    void on_l2Value_valueChanged(double arg1);
    void on_dValue_valueChanged(double arg1);
    void on_eValue_valueChanged(double arg1);
    void on_gammaValue_valueChanged(double arg1);
    void on_phiValue_valueChanged(double arg1);
    void on_gSoilValue_valueChanged(double arg1);
    void on_gammaValue_editingFinished();
    void on_analyzeButton_clicked();

    // menu actions
    void on_actionExit_triggered();

    // check boxes
    void on_chkBox_assume_rigid_cap_clicked(bool checked);
    void on_chkBox_include_toe_resistance_clicked(bool checked);

    void on_displacementSlider_sliderMoved(int position);

    // material table slots
    void updateInfo(QTableWidgetItem *);

private:
    Q_OBJECT
    Ui::MainWindow *ui;

    // get data
    double P;
    double L1;
    double L2;
    double D;
    double E;
    bool fixHead;
    int numEle;
    double gamma;
    double phi;
    double gSoil;
    int puSwitch;
    int kSwitch;
    int gwtSwitch;

    // states
    bool assumeRigidPileHead = false;
    bool useToeResistance    = false;

    // load parameter
    double displacementRatio;

    // soil layers and related methods
    QVector<soilLayer> soilLayers;

    void setupLayers();
    void reDrawTable();

};

#endif // MAINWINDOW_H
