#-------------------------------------------------
#
# Project created by QtCreator 2016-02-17T19:23:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = QtPile
TEMPLATE = app

#M_VERSION = 1.0.
#M_REV     = $Rev: $

PRODUCT_NAME = 'QtPile'

#DEFINES += APPLICATION_NAME=\"\\\"$$PRODUCT_NAME\\\"\"
#DEFINES += APPLICATION_VERSION=\"\\\"$$M_VERSION$$member(M_REV, 1)\\\"\"

#win32: INCLUDES += "./ops"
#win32: LIBS += -llapack -lblas
unix:  QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-variable
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
    surveysplashscreen.cpp
        
HEADERS  += mainwindow.h \
        qcustomplot.h \
    materialdbinterface.h \
    surveysplashscreen.h

# WHY DO WE NEED THIS ?????
HEADERS  += ui_mainwindow.h
        
FORMS    += mainwindow.ui \
    materialdbinterface.ui \
    surveysplashscreen.ui
        

RESOURCES += \
    qtpileresources.qrc
