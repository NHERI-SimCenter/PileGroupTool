#-------------------------------------------------
#
# Project created by QtCreator 2016-02-17T19:23:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = QtPile
TEMPLATE = app

include(OPS_includes.pro)

#INCLUDES += "/home/users/fmk/Downloads/Pile/ops"
#INCLUDEPATH += "$(HOME)/OpenSees/DEVELOPER/core"

SOURCES += main.cpp\
        mainwindow.cpp \
        getPyParam.cpp \
        getQzParam.cpp \
        getTZParam.cpp \
        qcustomplot.cpp
        
HEADERS  += mainwindow.h \
        qcustomplot.h
        
FORMS    += mainwindow.ui
        
