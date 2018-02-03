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

#include( $${PWD}/qwtconfig.pri )
#include ( $${PWD}/qwt.prf )

include( /usr/local/qwt-6.2.0-svn/features/qwtconfig.pri )
include( /usr/local/qwt-6.2.0-svn/features/qwt.prf )

#win32: INCLUDES += "./ops"
#win32: LIBS += -llapack -lblas
unix:  QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-variable -std=c++11
win32: QMAKE_CXXFLAGS += /Y-

include(OPS_includes.pro)

#INCLUDEPATH += "$(HOME)/OpenSees/DEVELOPER/core"

SOURCES += main.cpp\
        mainwindow.cpp \
        getPyParam.cpp \
        getQzParam.cpp \
        getTZParam.cpp \
        qcustomplot.cpp \
        materialdbinterface.cpp \
        surveysplashscreen.cpp \
        soilmat.cpp \
        utilWindows/copyrightdialog.cpp \
        utilWindows/dialogabout.cpp \
        utilWindows/dialogpreferences.cpp \
        utilWindows/dialogfuturefeature.cpp \
    ../widgets/Common/FooterWidget.cpp \
    ../widgets/Common/HeaderWidget.cpp \
    pilefeamodeler.cpp \
    systemplotqwt.cpp \
    systemplotsuper.cpp \
    systemplotqcp.cpp \
    resultplotsuper.cpp \
    resultplotqcp.cpp \
    resultplotqwt.cpp
        
HEADERS  += mainwindow.h \
        qcustomplot.h \
        materialdbinterface.h \
        surveysplashscreen.h \
        soilmat.h \
        utilWindows/copyrightdialog.h \
        utilWindows/dialogabout.h \
        utilWindows/dialogpreferences.h \
        utilWindows/dialogfuturefeature.h \
    ../widgets/Common/FooterWidget.h \
    ../widgets/Common/HeaderWidget.h \
    pilefeamodeler.h \
    pilegrouptool_parameters.h \
    systemplotqwt.h \
    systemplotsuper.h \
    systemplotqcp.h \
    resultplotsuper.h \
    resultplotqcp.h \
    resultplotqwt.h

FORMS    += mainwindow.ui \
        materialdbinterface.ui \
        surveysplashscreen.ui \
        utilWindows/copyrightdialog.ui \
        utilWindows/dialogabout.ui \
        utilWindows/dialogpreferences.ui \
        utilWindows/dialogfuturefeature.ui

RESOURCES += \
    qtpileresources.qrc

DISTFILES +=
