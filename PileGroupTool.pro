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
#     - in Qt Creator, PileGroupTool, rerun QMake and Build
#
# -- MacOS
#
#   * cannot open file 'qwt.lib'
#
#     first time compile only:
#     - open Terminal, navigate to your source folder
#     - cd qwt-6.1
#     - ~/Qt/5.10.0/clang_64/bin/qmake qwt.pro
#     - make
#
#     now, and with future updates, work as usual:
#     - return to Qt Creator and Build the PileGroupTool.
#
# -- Linux
#

QT       += core gui
unix: QT += svg opengl concurrent
win32: QT += opengl printsupport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport network

TARGET   = PileGroupTool
TEMPLATE = app
VERSION  = 2.1.0

#M_REV     = $Rev: $

PRODUCT_NAME = 'PileGroupTool'

#DEFINES += APPLICATION_NAME=\"\\\"$$PRODUCT_NAME\\\"\"
#DEFINES += APPLICATION_VERSION=\"\\\"$$M_VERSION$$member(M_REV, 1)\\\"\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

unix:  QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-variable -std=c++11
win32: QMAKE_CXXFLAGS += /Y- -wd"4100"

unix: DEPENDPATH += /usr/local/qwt-6.1.4
win32: DEPENDPATH += C:\Qwt-6.1.3

win32: include(C:\qwt-6.1.3\features\qwt.prf)
unix: include(/usr/local/qwt-6.1.4/features/qwt.prf)

include(OPS_includes.pro)

INCLUDEPATH += includes
INCLUDEPATH += mainWindow
INCLUDEPATH += widgets
INCLUDEPATH += dialogs
INCLUDEPATH += plots
INCLUDEPATH += FEA

SOURCES += main.cpp \
        mainWindow/mainwindow.cpp \
        FEA/getPyParam.cpp \
        FEA/getQzParam.cpp \
        FEA/getTZParam.cpp \
        FEA/soilmat.cpp \
        FEA/pilefeamodeler.cpp \
        dialogs/materialdbinterface.cpp \
        utilWindows/copyrightdialog.cpp \
        utilWindows/dialogabout.cpp \
        utilWindows/dialogpreferences.cpp \
        utilWindows/dialogfuturefeature.cpp \
        plots/systemplotsuper.cpp \
        plots/systemplotqwt.cpp \
        plots/resultplotsuper.cpp \
        plots/resultplotqwt.cpp \
        ../SimCenterCommon/Common/FooterWidget.cpp \
        ../SimCenterCommon/Common/HeaderWidget.cpp \
    widgets/sctrinputgroup.cpp \
    utilWindows/dialogquicktips.cpp
        
HEADERS  += \
        mainWindow/mainwindow.h \
        includes/pilegrouptool_parameters.h \
        FEA/soilmat.h \
        FEA/pilefeamodeler.h \
        dialogs/materialdbinterface.h \
        utilWindows/copyrightdialog.h \
        utilWindows/dialogabout.h \
        utilWindows/dialogpreferences.h \
        utilWindows/dialogfuturefeature.h \
        plots/systemplotsuper.h \
        plots/systemplotqwt.h \
        plots/resultplotsuper.h \
        plots/resultplotqwt.h \
        ../SimCenterCommon/Common/FooterWidget.h \
        ../SimCenterCommon/Common/HeaderWidget.h \
    widgets/sctrinputgroup.h \
    utilWindows/dialogquicktips.h

FORMS    += mainWindow/mainwindow.ui \
        dialogs/materialdbinterface.ui \
        utilWindows/copyrightdialog.ui \
        utilWindows/dialogabout.ui \
        utilWindows/dialogpreferences.ui \
        utilWindows/dialogfuturefeature.ui \
    widgets/sctrinputgroup.ui \
    utilWindows/dialogquicktips.ui

RESOURCES += \
	qtpileresources.qrc

DISTFILES +=
