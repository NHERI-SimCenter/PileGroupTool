#ifndef DIALOGPREFERENCES_H
#define DIALOGPREFERENCES_H

#include <QDialog>
#include <QAbstractButton>
#include <QSettings>

namespace Ui {
class DialogPreferences;
}

class DialogPreferences : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPreferences(QWidget *parent, QSettings *settings);
    ~DialogPreferences();
    void InitGUI();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_chkBx_displacement_stateChanged(int arg1);

    void on_chkBx_moment_stateChanged(int arg1);

    void on_chkBx_shear_stateChanged(int arg1);

    void on_chkBx_stress_stateChanged(int arg1);

    void on_chkBx_pu_stateChanged(int arg1);

    void on_chkBx_y50_stateChanged(int arg1);

    void on_buttonBox_accepted();

private:
    Ui::DialogPreferences *ui;

    // system conforming settings and parameters
    QSettings *DLGsettings;

    // viewer settings
    int DLGshowDisplacements;
    int DLGshowMoments;
    int DLGshowShear;
    int DLGshowStress;
    int DLGshowPultimate;
    int DLGshowY50;

    // meshing parameters
    int DLGminElementsPerLayer = 15;
    int DLGmaxElementsPerLayer = 40;
    int DLGnumElementsInAir    =  4;
    int DLGmaxNumberOfPiles    =  3;
};

#endif // DIALOGPREFERENCES_H
