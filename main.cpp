#include "mainwindow.h"
#include <QApplication>
#include <QResource>
#include <QFile>
//#include <QDir>


QString openStyleFiles()
{
    QString ret;
    QFile mainStyleFile(":/resources/styleSheets/pgtStyles.qss");

#ifdef Q_OS_WIN
    QFile appendedStyle(":/resources/styleSheets/pgtWin.qss");
#endif

#ifdef Q_OS_MACOS
    QFile appendedStyle(":/resources/styleSheets/pgtMac.qss");
#endif

#ifdef Q_OS_LINUX
    QFile appendedStyle(":/resources/styleSheets/pgtLinux.qss");
#endif

    if (!mainStyleFile.open(QFile::ReadOnly))
    {
        return ret;
    }

    if (!appendedStyle.open(QFile::ReadOnly))
    {
        return ret;
    }

    ret = ret.append(mainStyleFile.readAll());
    ret = ret.append(appendedStyle.readAll());

    mainStyleFile.close();
    appendedStyle.close();

    return ret;
}



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

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
    QApplication::setWindowIcon(QIcon(":/resources/NHERI-PGT-Icon.icns"));
    w.show();

    app.setStyleSheet(openStyleFiles());

    return app.exec();
}
