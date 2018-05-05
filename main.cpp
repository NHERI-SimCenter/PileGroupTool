#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include "surveysplashscreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //surveySplashScreen splash;
   // splash.exec();

    MainWindow w;
    w.setWindowIcon(QIcon(":/resources/NHERI-PGT-Icon.icns"));
    w.show();

    QFile file(":/style.qss");
    if(file.open(QFile::ReadOnly)) {
       QString styleSheet = QLatin1String(file.readAll());
       a.setStyleSheet(styleSheet);
    }

    return a.exec();
}
