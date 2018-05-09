#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include "surveysplashscreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool graphicModeQCP = false;
#ifdef USEQCP
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i],"-useQCP") == 0
                || strcmp(argv[i],"-QCP") == 0
                || strcmp(argv[i],"-useqcp") == 0
                || strcmp(argv[i],"-qcp") == 0)
        {
            graphicModeQCP = true;
        }
    }
#else
    graphicModeQCP = false;
#endif

    MainWindow w(graphicModeQCP);
    w.setWindowIcon(QIcon(":/resources/NHERI-PGT-Icon.icns"));
    w.show();

    QFile file(":/style.qss");
    if(file.open(QFile::ReadOnly)) {
       QString styleSheet = QLatin1String(file.readAll());
       a.setStyleSheet(styleSheet);
    }

    return a.exec();
}
