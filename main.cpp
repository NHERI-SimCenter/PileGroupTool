#include "MainWindow.h"
#include <QApplication>
#include "surveysplashscreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    surveySplashScreen splash;
    splash.exec();

    MainWindow w;
    w.show();

    return a.exec();
}
