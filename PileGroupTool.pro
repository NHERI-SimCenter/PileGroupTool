#-------------------------------------------------
#
# Project created by QtCreator 2016-02-17T19:23:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport network

TARGET = PileGroupTool
TEMPLATE = app
VERSION=1.99

#M_VERSION = 1.0.
#M_REV     = $Rev: $

PRODUCT_NAME = 'PileGroupTool'

#DEFINES += APPLICATION_NAME=\"\\\"$$PRODUCT_NAME\\\"\"
#DEFINES += APPLICATION_VERSION=\"\\\"$$M_VERSION$$member(M_REV, 1)\\\"\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

#include( ./qwt-6.2/qwtconfig.pri )
#include( ./qwt-6.2/qwt.prf )

#win32: INCLUDES += "./ops"
#win32: LIBS += -llapack -lblas
unix:  QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-variable -std=c++11
win32: QMAKE_CXXFLAGS += /Y-

include(OPS_includes.pro)

#INCLUDEPATH += "$(HOME)/OpenSees/DEVELOPER/core"
INCLUDEPATH += ./qwt-6.2/src
LIBS += -L"$(HOME)/Development/SimCenter/PileGroupTool/qwt-6.2/lib"
#LIBS += -L"$(HOME)/Documents/GitHub/PileGroupTool/qwt-6.2/lib"
LIBS += -lqwt

INCLUDEPATH += includes
INCLUDEPATH += mainWindow
INCLUDEPATH += dialogs
INCLUDEPATH += plots
INCLUDEPATH += FEA

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
