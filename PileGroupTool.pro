#-------------------------------------------------
#
# Project created by QtCreator 2016-02-17T19:23:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport network

TARGET = PileGroupTool
TEMPLATE = app
VERSION=1.0

#M_VERSION = 1.0.
#M_REV     = $Rev: $

PRODUCT_NAME = 'PileGroupTool'

#DEFINES += APPLICATION_NAME=\"\\\"$$PRODUCT_NAME\\\"\"
#DEFINES += APPLICATION_VERSION=\"\\\"$$M_VERSION$$member(M_REV, 1)\\\"\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

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
    ../widgets/Common/HeaderWidget.cpp
        
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
    ../widgets/Common/HeaderWidget.h

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
