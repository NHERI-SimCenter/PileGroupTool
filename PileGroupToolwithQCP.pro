#-------------------------------------------------
#
# Project created by QtCreator 2016-02-17T19:23:56
#
#-------------------------------------------------

## common compile issues
#
# -- Windows 10
#
#   * missing lapack.lib and/or blas.lib:
#
#     download and install Intel MKL (math kernel library).
#     it is free from intel's web site, easy to install, and efficient.
#
#   * LNK1104: cannot open file 'qwt.lib'
#
#     - in Qt Creator, open existing project PileGroupTool\qwt-6.2\qwt.pro
#     - configure the kit to compile in "[your_path]\build-qwt-[....]" (the full path is important)
#     - adjust the path in the LIBS line below to reflect your build directory
win32: {
    INCLUDEPATH += ./qwt-6.2/src
    #LIBS += -L"C:\Users\Peter Mackenzie\Documents\GitHub\build-qwt-Desktop_Qt_5_7_1_MSVC2015_64bit-Debug\lib"
    LIBS += -L"C:\Users\Peter Mackenzie\Documents\GitHub\build-qwt-Desktop_Qt_5_10_1_MSVC2015_64bit-Debug\lib"
    LIBS += qwt.lib
}
#     - in Qt Creator, PileGroupTool, rerun QMake and Build
#
# -- MacOS
#
#   * cannot open file 'qwt.lib'
#
#     first time compile only:
#     - open Terminal, navigate to your source folder
#     - cd qwt-6.2
#     - ~/Qt/5.10.0/clang_64/bin/qmake qwt.pro
#     - make
#
#     now, and with future updates, work as usual:
#     - return to Qt Creator and Build the PileGroupTool.
#
# -- Linux
#

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport network

TARGET   = PileGroupTool
TEMPLATE = app
VERSION  = 1.99.2

#VERSION = pre2.0.2
#M_REV     = $Rev: $

PRODUCT_NAME = 'PileGroupTool'

#DEFINES += APPLICATION_NAME=\"\\\"$$PRODUCT_NAME\\\"\"
#DEFINES += APPLICATION_VERSION=\"\\\"$$M_VERSION$$member(M_REV, 1)\\\"\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

unix:  QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-variable -std=c++11
win32: QMAKE_CXXFLAGS += /Y-

include(OPS_includes.pro)


#INCLUDEPATH += "$(HOME)/OpenSees/DEVELOPER/core"
INCLUDEPATH += ./qwt-6.2/src
LIBS += -L"$(HOME)/Development/SimCenter/PileGroupTool/qwt-6.2/lib"
LIBS += -L"$(HOME)/Documents/GitHub/PileGroupTool/qwt-6.2/lib"
LIBS += -lqwt

INCLUDEPATH += includes
INCLUDEPATH += mainWindow
INCLUDEPATH += dialogs
INCLUDEPATH += plots
INCLUDEPATH += FEA


unix: {
    INCLUDEPATH += ./qwt-6.2/src

    LIBS += -L"$(HOME)/Development/SimCenter/PileGroupTool/qwt-6.2/lib"
    LIBS += -lqwt
}

SOURCES += main.cpp\
        mainWindow/mainwindow.cpp \
        qcp/qcustomplot.cpp \
        FEA/getPyParam.cpp \
        FEA/getQzParam.cpp \
        FEA/getTZParam.cpp \
        FEA/soilmat.cpp \
        FEA/pilefeamodeler.cpp \
        dialogs/materialdbinterface.cpp \
        dialogs/surveysplashscreen.cpp \
        utilWindows/copyrightdialog.cpp \
        utilWindows/dialogabout.cpp \
        utilWindows/dialogpreferences.cpp \
        utilWindows/dialogfuturefeature.cpp \
        plots/systemplotsuper.cpp \
        plots/systemplotqcp.cpp \
        plots/systemplotqwt.cpp \
        plots/resultplotsuper.cpp \
        plots/resultplotqcp.cpp \
        plots/resultplotqwt.cpp \
        ../widgets/Common/FooterWidget.cpp \
        ../widgets/Common/HeaderWidget.cpp
        
HEADERS  += \
        mainWindow/mainwindow.h \
        includes/pilegrouptool_parameters.h \
        qcp/qcustomplot.h \
        FEA/soilmat.h \
        FEA/pilefeamodeler.h \
        dialogs/materialdbinterface.h \
        dialogs/surveysplashscreen.h \
        utilWindows/copyrightdialog.h \
        utilWindows/dialogabout.h \
        utilWindows/dialogpreferences.h \
        utilWindows/dialogfuturefeature.h \
        plots/systemplotsuper.h \
        plots/systemplotqcp.h \
        plots/systemplotqwt.h \
        plots/resultplotsuper.h \
        plots/resultplotqcp.h \
        plots/resultplotqwt.h \
        ../widgets/Common/FooterWidget.h \
        ../widgets/Common/HeaderWidget.h

FORMS    += mainWindow/mainwindow.ui \
        dialogs/materialdbinterface.ui \
        dialogs/surveysplashscreen.ui \
        utilWindows/copyrightdialog.ui \
        utilWindows/dialogabout.ui \
        utilWindows/dialogpreferences.ui \
        utilWindows/dialogfuturefeature.ui

RESOURCES += \
	qtpileresources.qrc

DISTFILES +=
