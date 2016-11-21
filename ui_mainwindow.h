/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave;
    QAction *actionNew;
    QAction *actionExit;
    QWidget *centralWidget;
    QTabWidget *properties;
    QWidget *pileProperties;
    QLabel *eLabel;
    QDoubleSpinBox *eValue;
    QDoubleSpinBox *dValue;
    QLabel *dLabel;
    QGroupBox *groupBox_2;
    QRadioButton *fixedHead;
    QRadioButton *pinnedHead;
    QWidget *soilProperties;
    QLabel *gammmaLabel;
    QLabel *phiLabel;
    QLabel *gSoilLabel;
    QDoubleSpinBox *gammaValue;
    QDoubleSpinBox *phiValue;
    QDoubleSpinBox *gSoilValue;
    QComboBox *puValue;
    QLabel *kLabel;
    QLabel *gwtLabel;
    QLabel *puLabel;
    QComboBox *kValue;
    QComboBox *gwtValue;
    QFrame *line;
    QFrame *line_2;
    QLabel *l1Label;
    QDoubleSpinBox *l1Value;
    QLabel *l2Label;
    QDoubleSpinBox *l2Value;
    QLabel *pLabel;
    QDoubleSpinBox *pValue;
    QFrame *line_3;
    QTabWidget *tabWidget;
    QWidget *displacement;
    QCustomPlot *displPlot;
    QWidget *shear;
    QCustomPlot *shearPlot;
    QWidget *moment;
    QCustomPlot *momentPlot;
    QMenuBar *menuBar;
    QMenu *menuOpenSeesPile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(606, 618);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        properties = new QTabWidget(centralWidget);
        properties->setObjectName(QStringLiteral("properties"));
        properties->setGeometry(QRect(0, 320, 231, 231));
        pileProperties = new QWidget();
        pileProperties->setObjectName(QStringLiteral("pileProperties"));
        eLabel = new QLabel(pileProperties);
        eLabel->setObjectName(QStringLiteral("eLabel"));
        eLabel->setGeometry(QRect(20, 120, 62, 19));
        eValue = new QDoubleSpinBox(pileProperties);
        eValue->setObjectName(QStringLiteral("eValue"));
        eValue->setGeometry(QRect(90, 120, 101, 24));
        eValue->setDecimals(0);
        eValue->setMinimum(1e+07);
        eValue->setMaximum(1e+08);
        eValue->setSingleStep(100000);
        eValue->setValue(2.5e+07);
        dValue = new QDoubleSpinBox(pileProperties);
        dValue->setObjectName(QStringLiteral("dValue"));
        dValue->setGeometry(QRect(120, 80, 70, 24));
        dValue->setMinimum(0.01);
        dValue->setSingleStep(0.01);
        dValue->setValue(1);
        dLabel = new QLabel(pileProperties);
        dLabel->setObjectName(QStringLiteral("dLabel"));
        dLabel->setGeometry(QRect(20, 90, 62, 19));
        groupBox_2 = new QGroupBox(pileProperties);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 10, 181, 61));
        fixedHead = new QRadioButton(groupBox_2);
        fixedHead->setObjectName(QStringLiteral("fixedHead"));
        fixedHead->setGeometry(QRect(10, 20, 102, 23));
        fixedHead->setChecked(true);
        pinnedHead = new QRadioButton(groupBox_2);
        pinnedHead->setObjectName(QStringLiteral("pinnedHead"));
        pinnedHead->setGeometry(QRect(90, 20, 102, 23));
        properties->addTab(pileProperties, QString());
        soilProperties = new QWidget();
        soilProperties->setObjectName(QStringLiteral("soilProperties"));
        gammmaLabel = new QLabel(soilProperties);
        gammmaLabel->setObjectName(QStringLiteral("gammmaLabel"));
        gammmaLabel->setGeometry(QRect(10, 10, 51, 19));
        phiLabel = new QLabel(soilProperties);
        phiLabel->setObjectName(QStringLiteral("phiLabel"));
        phiLabel->setGeometry(QRect(10, 40, 51, 19));
        gSoilLabel = new QLabel(soilProperties);
        gSoilLabel->setObjectName(QStringLiteral("gSoilLabel"));
        gSoilLabel->setGeometry(QRect(10, 70, 51, 19));
        gammaValue = new QDoubleSpinBox(soilProperties);
        gammaValue->setObjectName(QStringLiteral("gammaValue"));
        gammaValue->setGeometry(QRect(70, 10, 70, 24));
        gammaValue->setMinimum(1);
        gammaValue->setValue(17);
        phiValue = new QDoubleSpinBox(soilProperties);
        phiValue->setObjectName(QStringLiteral("phiValue"));
        phiValue->setGeometry(QRect(70, 40, 70, 24));
        phiValue->setValue(36);
        gSoilValue = new QDoubleSpinBox(soilProperties);
        gSoilValue->setObjectName(QStringLiteral("gSoilValue"));
        gSoilValue->setGeometry(QRect(70, 70, 70, 24));
        gSoilValue->setDecimals(0);
        gSoilValue->setMinimum(10);
        gSoilValue->setMaximum(100001);
        gSoilValue->setSingleStep(100);
        gSoilValue->setValue(15000);
        puValue = new QComboBox(soilProperties);
        puValue->setObjectName(QStringLiteral("puValue"));
        puValue->setGeometry(QRect(70, 100, 71, 21));
        puValue->setCurrentText(QStringLiteral("API"));
        kLabel = new QLabel(soilProperties);
        kLabel->setObjectName(QStringLiteral("kLabel"));
        kLabel->setGeometry(QRect(10, 130, 62, 19));
        gwtLabel = new QLabel(soilProperties);
        gwtLabel->setObjectName(QStringLiteral("gwtLabel"));
        gwtLabel->setGeometry(QRect(10, 160, 62, 19));
        puLabel = new QLabel(soilProperties);
        puLabel->setObjectName(QStringLiteral("puLabel"));
        puLabel->setGeometry(QRect(10, 100, 62, 19));
        kValue = new QComboBox(soilProperties);
        kValue->setObjectName(QStringLiteral("kValue"));
        kValue->setGeometry(QRect(70, 130, 71, 21));
        gwtValue = new QComboBox(soilProperties);
        gwtValue->setObjectName(QStringLiteral("gwtValue"));
        gwtValue->setGeometry(QRect(70, 160, 71, 21));
        properties->addTab(soilProperties, QString());
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(70, 80, 118, 3));
        line->setFrameShadow(QFrame::Plain);
        line->setLineWidth(2);
        line->setFrameShape(QFrame::HLine);
        line_2 = new QFrame(centralWidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(120, 30, 20, 271));
        line_2->setFrameShadow(QFrame::Plain);
        line_2->setLineWidth(5);
        line_2->setFrameShape(QFrame::VLine);
        l1Label = new QLabel(centralWidget);
        l1Label->setObjectName(QStringLiteral("l1Label"));
        l1Label->setGeometry(QRect(100, 40, 21, 19));
        l1Value = new QDoubleSpinBox(centralWidget);
        l1Value->setObjectName(QStringLiteral("l1Value"));
        l1Value->setGeometry(QRect(150, 50, 70, 24));
        l1Value->setDecimals(1);
        l1Value->setValue(1);
        l2Label = new QLabel(centralWidget);
        l2Label->setObjectName(QStringLiteral("l2Label"));
        l2Label->setGeometry(QRect(100, 190, 21, 19));
        l2Value = new QDoubleSpinBox(centralWidget);
        l2Value->setObjectName(QStringLiteral("l2Value"));
        l2Value->setGeometry(QRect(140, 190, 70, 24));
        l2Value->setDecimals(1);
        l2Value->setMinimum(1);
        l2Value->setMaximum(201);
        l2Value->setValue(20);
        pLabel = new QLabel(centralWidget);
        pLabel->setObjectName(QStringLiteral("pLabel"));
        pLabel->setGeometry(QRect(40, 10, 16, 21));
        pValue = new QDoubleSpinBox(centralWidget);
        pValue->setObjectName(QStringLiteral("pValue"));
        pValue->setGeometry(QRect(140, 20, 81, 24));
        pValue->setDecimals(0);
        pValue->setMaximum(10001);
        pValue->setValue(3500);
        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(60, 10, 71, 41));
        line_3->setLineWidth(3);
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(240, 0, 351, 551));
        displacement = new QWidget();
        displacement->setObjectName(QStringLiteral("displacement"));
        displPlot = new QCustomPlot(displacement);
        displPlot->setObjectName(QStringLiteral("displPlot"));
        displPlot->setGeometry(QRect(9, 9, 331, 521));
        tabWidget->addTab(displacement, QString());
        shear = new QWidget();
        shear->setObjectName(QStringLiteral("shear"));
        shearPlot = new QCustomPlot(shear);
        shearPlot->setObjectName(QStringLiteral("shearPlot"));
        shearPlot->setGeometry(QRect(9, 9, 331, 451));
        tabWidget->addTab(shear, QString());
        moment = new QWidget();
        moment->setObjectName(QStringLiteral("moment"));
        momentPlot = new QCustomPlot(moment);
        momentPlot->setObjectName(QStringLiteral("momentPlot"));
        momentPlot->setGeometry(QRect(9, 9, 331, 451));
        tabWidget->addTab(moment, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 606, 22));
        menuOpenSeesPile = new QMenu(menuBar);
        menuOpenSeesPile->setObjectName(QStringLiteral("menuOpenSeesPile"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuOpenSeesPile->menuAction());
        menuOpenSeesPile->addAction(actionNew);
        menuOpenSeesPile->addAction(actionSave);
        menuOpenSeesPile->addAction(actionExit);

        retranslateUi(MainWindow);

        properties->setCurrentIndex(0);
        puValue->setCurrentIndex(0);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionSave->setText(QApplication::translate("MainWindow", "Save", 0));
        actionNew->setText(QApplication::translate("MainWindow", "New", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        eLabel->setText(QApplication::translate("MainWindow", "E", 0));
        dLabel->setText(QApplication::translate("MainWindow", "Diameter", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Pile Head", 0));
        fixedHead->setText(QApplication::translate("MainWindow", "Fixed", 0));
        pinnedHead->setText(QApplication::translate("MainWindow", "Pinned", 0));
        properties->setTabText(properties->indexOf(pileProperties), QApplication::translate("MainWindow", "Pile Properties", 0));
        gammmaLabel->setText(QApplication::translate("MainWindow", "gamma", 0));
        phiLabel->setText(QApplication::translate("MainWindow", "phi", 0));
        gSoilLabel->setText(QApplication::translate("MainWindow", "Gsoil", 0));
        puValue->clear();
        puValue->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "API", 0)
         << QApplication::translate("MainWindow", "Hansen", 0)
        );
        kLabel->setText(QApplication::translate("MainWindow", "k", 0));
        gwtLabel->setText(QApplication::translate("MainWindow", "gwt", 0));
        puLabel->setText(QApplication::translate("MainWindow", "pu", 0));
        kValue->clear();
        kValue->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "API", 0)
         << QApplication::translate("MainWindow", "Boulanger", 0)
        );
        gwtValue->clear();
        gwtValue->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "above", 0)
         << QApplication::translate("MainWindow", "below", 0)
        );
        properties->setTabText(properties->indexOf(soilProperties), QApplication::translate("MainWindow", "Soil Properties", 0));
        l1Label->setText(QApplication::translate("MainWindow", "L1", 0));
        l2Label->setText(QApplication::translate("MainWindow", "L2", 0));
        pLabel->setText(QApplication::translate("MainWindow", "P", 0));
        tabWidget->setTabText(tabWidget->indexOf(displacement), QApplication::translate("MainWindow", "Displacement", 0));
        tabWidget->setTabText(tabWidget->indexOf(shear), QApplication::translate("MainWindow", "Shear", 0));
        tabWidget->setTabText(tabWidget->indexOf(moment), QApplication::translate("MainWindow", "Moment", 0));
        menuOpenSeesPile->setTitle(QApplication::translate("MainWindow", "OpenSeesPile", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
