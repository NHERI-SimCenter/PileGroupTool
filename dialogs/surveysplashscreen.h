#ifndef SURVEYSPLASHSCREEN_H
#define SURVEYSPLASHSCREEN_H

#include <QDialog>

namespace Ui {
class surveySplashScreen;
}

class surveySplashScreen : public QDialog
{
    Q_OBJECT

public:
    explicit surveySplashScreen(QWidget *parent = 0);
    ~surveySplashScreen();

private slots:
    void on_pushButton_OK_clicked();

private:
    Ui::surveySplashScreen *ui;
};

#endif // SURVEYSPLASHSCREEN_H
