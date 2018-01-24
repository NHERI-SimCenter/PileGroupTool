/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "../widgets/Common/FooterWidget.h"
#include "../widgets/Common/HeaderWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave;
    QAction *actionNew;
    QAction *actionExit;
    QAction *actionReset;
    QAction *actionFEA_parameters;
    QAction *action_Open;
    QAction *actionExport_to_OpenSees;
    QAction *actionLicense_Information;
    QAction *actionPreferences;
    QAction *action_About;
    QAction *actionProvide_Feedback;
    QAction *actionVersion;
    QAction *actionLicense;
    QWidget *centralWidget;
    QGridLayout *gridLayout_11;
    HeaderWidget *headerWidget;
    FooterWidget *footerWidget;
    QVBoxLayout *verticalLayout;
    QFrame *line_3;
    QTextBrowser *textBrowser;
    QTabWidget *properties;
    QWidget *pilePropertiesWidget;
    QGridLayout *gridLayout;
    QFrame *pileProperties;
    QGridLayout *gridLayout_12;
    QSpinBox *pileIndex;
    QCheckBox *chkBox_include_toe_resistance;
    QCheckBox *chkBox_assume_rigid_cap;
    QLabel *lengthLabel;
    QLabel *label_7;
    QLabel *label_5;
    QDoubleSpinBox *Emodulus;
    QLabel *emodulusLabel;
    QLabel *freeLengthLabel;
    QLabel *label_2;
    QLabel *diameterLabel;
    QLabel *label_3;
    QLabel *pilePropertyLabel;
    QPushButton *btn_deletePile;
    QDoubleSpinBox *freeLength;
    QDoubleSpinBox *xOffset;
    QPushButton *btn_newPile;
    QLabel *label_4;
    QDoubleSpinBox *pileDiameter;
    QDoubleSpinBox *embeddedLength;
    QFrame *line;
    QLabel *label_6;
    QLabel *label;
    QSpacerItem *verticalSpacer;
    QWidget *soilPropertiesWidget;
    QVBoxLayout *verticalLayout_2;
    QFrame *soilProperties;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_GWT;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_depth;
    QDoubleSpinBox *groundWaterTable;
    QLabel *label_9;
    QLabel *label_Layers;
    QFrame *layerSelection;
    QGridLayout *gridLayout_9;
    QCheckBox *chkBox_layer2;
    QCheckBox *chkBox_layer1;
    QCheckBox *chkBox_layer3;
    QLabel *labelChangingLayer;
    QFrame *propertiesFrame;
    QGridLayout *gridLayout_10;
    QDoubleSpinBox *layerThickness;
    QLabel *label_12;
    QDoubleSpinBox *layerShearModulus;
    QLabel *label_15;
    QLabel *label_16;
    QDoubleSpinBox *layerSaturatedWeight;
    QLabel *label_13;
    QLabel *label_18;
    QLabel *label_17;
    QDoubleSpinBox *layerDryWeight;
    QLabel *label_14;
    QDoubleSpinBox *layerFrictionAngle;
    QLabel *label_19;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *labelLayerProperties;
    QWidget *loadPropertyWidget;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_20;
    QComboBox *forceTypeSelector;
    QStackedWidget *loadTypesStack;
    QWidget *page_forceControl;
    QGridLayout *gridLayout_16;
    QFrame *loadProperties;
    QGridLayout *gridLayout_14;
    QLabel *forcelabelVert;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_21;
    QSlider *horizontalForceSlider;
    QLabel *label_8;
    QSlider *verticalForceSlider;
    QLabel *forceLabelHoriz;
    QDoubleSpinBox *appliedVerticalForce;
    QLabel *label_23;
    QDoubleSpinBox *appliedMoment;
    QLabel *forceLabelMoment;
    QDoubleSpinBox *appliedHorizontalForce;
    QSlider *momentSlider;
    QWidget *page_pushOver;
    QGridLayout *gridLayout_17;
    QFrame *pushOverFrame;
    QGridLayout *gridLayout_18;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_pullout;
    QLabel *label_unitPushover;
    QDoubleSpinBox *pushoverDisplacement;
    QDoubleSpinBox *pulloutDisplacement;
    QLabel *label_pushover;
    QSlider *pushoverDisplacementSlider;
    QLabel *label_unitPullout;
    QSlider *pulloutDisplacementSlider;
    QWidget *page_displacementProfile;
    QGridLayout *gridLayout_19;
    QFrame *frame;
    QGridLayout *gridLayout_20;
    QLabel *label_BaseDisplacement;
    QLabel *label_Interface23;
    QLabel *label_SurfaceDisplacement;
    QSpinBox *Interface23;
    QLabel *label_Interface12;
    QDoubleSpinBox *surfaceDisplacement;
    QSpinBox *Interface12;
    QSpinBox *BaseDisplacement;
    QSlider *Interface12Slider;
    QLabel *label_unitInterface12;
    QLabel *label_unitSurfaceDisplacement;
    QSlider *surfaceDisplacementSlider;
    QLabel *label_unitInterface23;
    QSlider *Interface23Slider;
    QSlider *BaseDisplacementSlider;
    QLabel *label_unitBaseDisplacement;
    QSpacerItem *verticalSpacer_4;
    QTabWidget *tabWidget;
    QWidget *dispTab;
    QGridLayout *gridLayout_3;
    QWidget *pulloutTab;
    QGridLayout *gridLayout_15;
    QWidget *momentTab;
    QGridLayout *gridLayout_5;
    QWidget *shearTab;
    QGridLayout *gridLayout_4;
    QWidget *axialTab;
    QGridLayout *gridLayout_13;
    QWidget *stressTab;
    QGridLayout *gridLayout_6;
    QWidget *pultTab;
    QGridLayout *gridLayout_7;
    QWidget *y50Tab;
    QGridLayout *gridLayout_8;
    QTabWidget *tabWidget_system;
    QWidget *systemTab;
    QGridLayout *gridLayout_2;
    QMenuBar *menuBar;
    QMenu *menuOpenSeesPile;
    QMenu *menuAnalysis;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(994, 774);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(900, 600));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionReset = new QAction(MainWindow);
        actionReset->setObjectName(QStringLiteral("actionReset"));
        actionFEA_parameters = new QAction(MainWindow);
        actionFEA_parameters->setObjectName(QStringLiteral("actionFEA_parameters"));
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QStringLiteral("action_Open"));
        actionExport_to_OpenSees = new QAction(MainWindow);
        actionExport_to_OpenSees->setObjectName(QStringLiteral("actionExport_to_OpenSees"));
        actionLicense_Information = new QAction(MainWindow);
        actionLicense_Information->setObjectName(QStringLiteral("actionLicense_Information"));
        actionPreferences = new QAction(MainWindow);
        actionPreferences->setObjectName(QStringLiteral("actionPreferences"));
        action_About = new QAction(MainWindow);
        action_About->setObjectName(QStringLiteral("action_About"));
        actionProvide_Feedback = new QAction(MainWindow);
        actionProvide_Feedback->setObjectName(QStringLiteral("actionProvide_Feedback"));
        actionVersion = new QAction(MainWindow);
        actionVersion->setObjectName(QStringLiteral("actionVersion"));
        actionLicense = new QAction(MainWindow);
        actionLicense->setObjectName(QStringLiteral("actionLicense"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_11 = new QGridLayout(centralWidget);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        headerWidget = new HeaderWidget(centralWidget);
        headerWidget->setObjectName(QStringLiteral("headerWidget"));
        headerWidget->setMinimumSize(QSize(0, 0));

        gridLayout_11->addWidget(headerWidget, 0, 0, 1, 5);

        footerWidget = new FooterWidget(centralWidget);
        footerWidget->setObjectName(QStringLiteral("footerWidget"));
        footerWidget->setMinimumSize(QSize(0, 0));

        gridLayout_11->addWidget(footerWidget, 2, 0, 1, 5);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        line_3 = new QFrame(centralWidget);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_3);

        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy1);
        QFont font;
        font.setPointSize(12);
        textBrowser->setFont(font);
        textBrowser->setAcceptRichText(false);

        verticalLayout->addWidget(textBrowser);

        properties = new QTabWidget(centralWidget);
        properties->setObjectName(QStringLiteral("properties"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(properties->sizePolicy().hasHeightForWidth());
        properties->setSizePolicy(sizePolicy2);
        properties->setMinimumSize(QSize(300, 300));
        properties->setTabPosition(QTabWidget::West);
        properties->setUsesScrollButtons(false);
        pilePropertiesWidget = new QWidget();
        pilePropertiesWidget->setObjectName(QStringLiteral("pilePropertiesWidget"));
        gridLayout = new QGridLayout(pilePropertiesWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pileProperties = new QFrame(pilePropertiesWidget);
        pileProperties->setObjectName(QStringLiteral("pileProperties"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(2);
        sizePolicy3.setHeightForWidth(pileProperties->sizePolicy().hasHeightForWidth());
        pileProperties->setSizePolicy(sizePolicy3);
        pileProperties->setFrameShape(QFrame::StyledPanel);
        pileProperties->setFrameShadow(QFrame::Raised);
        gridLayout_12 = new QGridLayout(pileProperties);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        gridLayout_12->setContentsMargins(12, 12, 12, 12);
        pileIndex = new QSpinBox(pileProperties);
        pileIndex->setObjectName(QStringLiteral("pileIndex"));
        pileIndex->setMinimum(1);
        pileIndex->setMaximum(5);

        gridLayout_12->addWidget(pileIndex, 1, 1, 1, 1);

        chkBox_include_toe_resistance = new QCheckBox(pileProperties);
        chkBox_include_toe_resistance->setObjectName(QStringLiteral("chkBox_include_toe_resistance"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(chkBox_include_toe_resistance->sizePolicy().hasHeightForWidth());
        chkBox_include_toe_resistance->setSizePolicy(sizePolicy4);
        chkBox_include_toe_resistance->setMinimumSize(QSize(0, 30));

        gridLayout_12->addWidget(chkBox_include_toe_resistance, 9, 0, 1, 2);

        chkBox_assume_rigid_cap = new QCheckBox(pileProperties);
        chkBox_assume_rigid_cap->setObjectName(QStringLiteral("chkBox_assume_rigid_cap"));
        sizePolicy4.setHeightForWidth(chkBox_assume_rigid_cap->sizePolicy().hasHeightForWidth());
        chkBox_assume_rigid_cap->setSizePolicy(sizePolicy4);
        chkBox_assume_rigid_cap->setMinimumSize(QSize(0, 30));

        gridLayout_12->addWidget(chkBox_assume_rigid_cap, 8, 0, 1, 2);

        lengthLabel = new QLabel(pileProperties);
        lengthLabel->setObjectName(QStringLiteral("lengthLabel"));

        gridLayout_12->addWidget(lengthLabel, 4, 0, 1, 1);

        label_7 = new QLabel(pileProperties);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_12->addWidget(label_7, 6, 2, 1, 1);

        label_5 = new QLabel(pileProperties);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_12->addWidget(label_5, 4, 2, 1, 1);

        Emodulus = new QDoubleSpinBox(pileProperties);
        Emodulus->setObjectName(QStringLiteral("Emodulus"));
        Emodulus->setDecimals(0);
        Emodulus->setMinimum(1);
        Emodulus->setMaximum(1000);
        Emodulus->setSingleStep(1);
        Emodulus->setValue(20);

        gridLayout_12->addWidget(Emodulus, 6, 1, 1, 1);

        emodulusLabel = new QLabel(pileProperties);
        emodulusLabel->setObjectName(QStringLiteral("emodulusLabel"));

        gridLayout_12->addWidget(emodulusLabel, 6, 0, 1, 1);

        freeLengthLabel = new QLabel(pileProperties);
        freeLengthLabel->setObjectName(QStringLiteral("freeLengthLabel"));

        gridLayout_12->addWidget(freeLengthLabel, 5, 0, 1, 1);

        label_2 = new QLabel(pileProperties);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_12->addWidget(label_2, 2, 0, 1, 1);

        diameterLabel = new QLabel(pileProperties);
        diameterLabel->setObjectName(QStringLiteral("diameterLabel"));

        gridLayout_12->addWidget(diameterLabel, 3, 0, 1, 1);

        label_3 = new QLabel(pileProperties);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_12->addWidget(label_3, 2, 2, 1, 1);

        pilePropertyLabel = new QLabel(pileProperties);
        pilePropertyLabel->setObjectName(QStringLiteral("pilePropertyLabel"));
        pilePropertyLabel->setMaximumSize(QSize(16777215, 25));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        pilePropertyLabel->setFont(font1);

        gridLayout_12->addWidget(pilePropertyLabel, 0, 0, 1, 1);

        btn_deletePile = new QPushButton(pileProperties);
        btn_deletePile->setObjectName(QStringLiteral("btn_deletePile"));

        gridLayout_12->addWidget(btn_deletePile, 12, 0, 1, 1);

        freeLength = new QDoubleSpinBox(pileProperties);
        freeLength->setObjectName(QStringLiteral("freeLength"));

        gridLayout_12->addWidget(freeLength, 5, 1, 1, 1);

        xOffset = new QDoubleSpinBox(pileProperties);
        xOffset->setObjectName(QStringLiteral("xOffset"));
        xOffset->setMinimum(-99.99);

        gridLayout_12->addWidget(xOffset, 2, 1, 1, 1);

        btn_newPile = new QPushButton(pileProperties);
        btn_newPile->setObjectName(QStringLiteral("btn_newPile"));

        gridLayout_12->addWidget(btn_newPile, 12, 1, 1, 2);

        label_4 = new QLabel(pileProperties);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_12->addWidget(label_4, 3, 2, 1, 1);

        pileDiameter = new QDoubleSpinBox(pileProperties);
        pileDiameter->setObjectName(QStringLiteral("pileDiameter"));
        pileDiameter->setMinimum(0.05);
        pileDiameter->setSingleStep(0.05);
        pileDiameter->setValue(1);

        gridLayout_12->addWidget(pileDiameter, 3, 1, 1, 1);

        embeddedLength = new QDoubleSpinBox(pileProperties);
        embeddedLength->setObjectName(QStringLiteral("embeddedLength"));

        gridLayout_12->addWidget(embeddedLength, 4, 1, 1, 1);

        line = new QFrame(pileProperties);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_12->addWidget(line, 7, 0, 1, 3);

        label_6 = new QLabel(pileProperties);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_12->addWidget(label_6, 5, 2, 1, 1);

        label = new QLabel(pileProperties);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_12->addWidget(label, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_12->addItem(verticalSpacer, 10, 0, 1, 1);


        gridLayout->addWidget(pileProperties, 1, 0, 1, 1);

        properties->addTab(pilePropertiesWidget, QString());
        soilPropertiesWidget = new QWidget();
        soilPropertiesWidget->setObjectName(QStringLiteral("soilPropertiesWidget"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(1);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(soilPropertiesWidget->sizePolicy().hasHeightForWidth());
        soilPropertiesWidget->setSizePolicy(sizePolicy5);
        verticalLayout_2 = new QVBoxLayout(soilPropertiesWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        soilProperties = new QFrame(soilPropertiesWidget);
        soilProperties->setObjectName(QStringLiteral("soilProperties"));
        soilProperties->setFrameShape(QFrame::StyledPanel);
        soilProperties->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(soilProperties);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_GWT = new QLabel(soilProperties);
        label_GWT->setObjectName(QStringLiteral("label_GWT"));
        label_GWT->setMinimumSize(QSize(0, 20));
        label_GWT->setMaximumSize(QSize(16777215, 30));
        label_GWT->setFont(font1);

        verticalLayout_3->addWidget(label_GWT);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_depth = new QLabel(soilProperties);
        label_depth->setObjectName(QStringLiteral("label_depth"));
        label_depth->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_depth);

        groundWaterTable = new QDoubleSpinBox(soilProperties);
        groundWaterTable->setObjectName(QStringLiteral("groundWaterTable"));

        horizontalLayout_4->addWidget(groundWaterTable);

        label_9 = new QLabel(soilProperties);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_4->addWidget(label_9);


        verticalLayout_3->addLayout(horizontalLayout_4);

        label_Layers = new QLabel(soilProperties);
        label_Layers->setObjectName(QStringLiteral("label_Layers"));
        label_Layers->setMinimumSize(QSize(0, 20));
        label_Layers->setMaximumSize(QSize(16777215, 30));
        label_Layers->setFont(font1);

        verticalLayout_3->addWidget(label_Layers);

        layerSelection = new QFrame(soilProperties);
        layerSelection->setObjectName(QStringLiteral("layerSelection"));
        gridLayout_9 = new QGridLayout(layerSelection);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        gridLayout_9->setContentsMargins(0, 0, 0, 0);
        chkBox_layer2 = new QCheckBox(layerSelection);
        chkBox_layer2->setObjectName(QStringLiteral("chkBox_layer2"));
        chkBox_layer2->setAutoExclusive(true);

        gridLayout_9->addWidget(chkBox_layer2, 1, 1, 1, 1);

        chkBox_layer1 = new QCheckBox(layerSelection);
        chkBox_layer1->setObjectName(QStringLiteral("chkBox_layer1"));
        chkBox_layer1->setAutoExclusive(true);

        gridLayout_9->addWidget(chkBox_layer1, 1, 0, 1, 1);

        chkBox_layer3 = new QCheckBox(layerSelection);
        chkBox_layer3->setObjectName(QStringLiteral("chkBox_layer3"));
        chkBox_layer3->setAutoExclusive(true);

        gridLayout_9->addWidget(chkBox_layer3, 1, 2, 1, 1);

        labelChangingLayer = new QLabel(layerSelection);
        labelChangingLayer->setObjectName(QStringLiteral("labelChangingLayer"));

        gridLayout_9->addWidget(labelChangingLayer, 0, 0, 1, 3);


        verticalLayout_3->addWidget(layerSelection);

        propertiesFrame = new QFrame(soilProperties);
        propertiesFrame->setObjectName(QStringLiteral("propertiesFrame"));
        gridLayout_10 = new QGridLayout(propertiesFrame);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        gridLayout_10->setHorizontalSpacing(12);
        gridLayout_10->setContentsMargins(6, -1, 6, -1);
        layerThickness = new QDoubleSpinBox(propertiesFrame);
        layerThickness->setObjectName(QStringLiteral("layerThickness"));

        gridLayout_10->addWidget(layerThickness, 1, 1, 1, 1);

        label_12 = new QLabel(propertiesFrame);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_10->addWidget(label_12, 1, 0, 1, 1);

        layerShearModulus = new QDoubleSpinBox(propertiesFrame);
        layerShearModulus->setObjectName(QStringLiteral("layerShearModulus"));
        layerShearModulus->setDecimals(1);
        layerShearModulus->setMaximum(1000);
        layerShearModulus->setSingleStep(10);

        gridLayout_10->addWidget(layerShearModulus, 6, 1, 1, 1);

        label_15 = new QLabel(propertiesFrame);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_10->addWidget(label_15, 2, 2, 1, 1);

        label_16 = new QLabel(propertiesFrame);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_10->addWidget(label_16, 3, 2, 1, 1);

        layerSaturatedWeight = new QDoubleSpinBox(propertiesFrame);
        layerSaturatedWeight->setObjectName(QStringLiteral("layerSaturatedWeight"));

        gridLayout_10->addWidget(layerSaturatedWeight, 3, 1, 1, 1);

        label_13 = new QLabel(propertiesFrame);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_10->addWidget(label_13, 5, 0, 1, 1);

        label_18 = new QLabel(propertiesFrame);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout_10->addWidget(label_18, 5, 2, 1, 1);

        label_17 = new QLabel(propertiesFrame);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_10->addWidget(label_17, 1, 2, 1, 1);

        layerDryWeight = new QDoubleSpinBox(propertiesFrame);
        layerDryWeight->setObjectName(QStringLiteral("layerDryWeight"));

        gridLayout_10->addWidget(layerDryWeight, 2, 1, 1, 1);

        label_14 = new QLabel(propertiesFrame);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_10->addWidget(label_14, 6, 0, 1, 1);

        layerFrictionAngle = new QDoubleSpinBox(propertiesFrame);
        layerFrictionAngle->setObjectName(QStringLiteral("layerFrictionAngle"));
        layerFrictionAngle->setDecimals(1);
        layerFrictionAngle->setMaximum(60);

        gridLayout_10->addWidget(layerFrictionAngle, 5, 1, 1, 1);

        label_19 = new QLabel(propertiesFrame);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_10->addWidget(label_19, 6, 2, 1, 1);

        label_10 = new QLabel(propertiesFrame);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout_10->addWidget(label_10, 2, 0, 1, 1);

        label_11 = new QLabel(propertiesFrame);
        label_11->setObjectName(QStringLiteral("label_11"));
        QSizePolicy sizePolicy6(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy6);

        gridLayout_10->addWidget(label_11, 3, 0, 1, 1);

        labelLayerProperties = new QLabel(propertiesFrame);
        labelLayerProperties->setObjectName(QStringLiteral("labelLayerProperties"));

        gridLayout_10->addWidget(labelLayerProperties, 0, 0, 1, 3);

        gridLayout_10->setColumnStretch(0, 3);
        gridLayout_10->setColumnStretch(1, 1);

        verticalLayout_3->addWidget(propertiesFrame);


        verticalLayout_2->addWidget(soilProperties);

        properties->addTab(soilPropertiesWidget, QString());
        loadPropertyWidget = new QWidget();
        loadPropertyWidget->setObjectName(QStringLiteral("loadPropertyWidget"));
        verticalLayout_5 = new QVBoxLayout(loadPropertyWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_20 = new QLabel(loadPropertyWidget);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setFont(font1);

        verticalLayout_5->addWidget(label_20);

        forceTypeSelector = new QComboBox(loadPropertyWidget);
        forceTypeSelector->addItem(QString());
        forceTypeSelector->addItem(QString());
        forceTypeSelector->addItem(QString());
        forceTypeSelector->setObjectName(QStringLiteral("forceTypeSelector"));

        verticalLayout_5->addWidget(forceTypeSelector);

        loadTypesStack = new QStackedWidget(loadPropertyWidget);
        loadTypesStack->setObjectName(QStringLiteral("loadTypesStack"));
        page_forceControl = new QWidget();
        page_forceControl->setObjectName(QStringLiteral("page_forceControl"));
        gridLayout_16 = new QGridLayout(page_forceControl);
        gridLayout_16->setSpacing(6);
        gridLayout_16->setContentsMargins(11, 11, 11, 11);
        gridLayout_16->setObjectName(QStringLiteral("gridLayout_16"));
        gridLayout_16->setContentsMargins(0, 0, 0, 0);
        loadProperties = new QFrame(page_forceControl);
        loadProperties->setObjectName(QStringLiteral("loadProperties"));
        loadProperties->setMinimumSize(QSize(0, 0));
        loadProperties->setFrameShape(QFrame::StyledPanel);
        loadProperties->setFrameShadow(QFrame::Raised);
        gridLayout_14 = new QGridLayout(loadProperties);
        gridLayout_14->setSpacing(6);
        gridLayout_14->setContentsMargins(11, 11, 11, 11);
        gridLayout_14->setObjectName(QStringLiteral("gridLayout_14"));
        forcelabelVert = new QLabel(loadProperties);
        forcelabelVert->setObjectName(QStringLiteral("forcelabelVert"));

        gridLayout_14->addWidget(forcelabelVert, 4, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_14->addItem(verticalSpacer_2, 8, 1, 1, 1);

        label_21 = new QLabel(loadProperties);
        label_21->setObjectName(QStringLiteral("label_21"));

        gridLayout_14->addWidget(label_21, 4, 2, 1, 1);

        horizontalForceSlider = new QSlider(loadProperties);
        horizontalForceSlider->setObjectName(QStringLiteral("horizontalForceSlider"));
        QSizePolicy sizePolicy7(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(horizontalForceSlider->sizePolicy().hasHeightForWidth());
        horizontalForceSlider->setSizePolicy(sizePolicy7);
        horizontalForceSlider->setMinimumSize(QSize(0, 30));
        horizontalForceSlider->setMinimum(-100);
        horizontalForceSlider->setMaximum(100);
        horizontalForceSlider->setOrientation(Qt::Horizontal);
        horizontalForceSlider->setTickPosition(QSlider::NoTicks);

        gridLayout_14->addWidget(horizontalForceSlider, 3, 1, 1, 1);

        label_8 = new QLabel(loadProperties);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_14->addWidget(label_8, 2, 2, 1, 1);

        verticalForceSlider = new QSlider(loadProperties);
        verticalForceSlider->setObjectName(QStringLiteral("verticalForceSlider"));
        verticalForceSlider->setOrientation(Qt::Horizontal);

        gridLayout_14->addWidget(verticalForceSlider, 5, 1, 1, 1);

        forceLabelHoriz = new QLabel(loadProperties);
        forceLabelHoriz->setObjectName(QStringLiteral("forceLabelHoriz"));

        gridLayout_14->addWidget(forceLabelHoriz, 2, 0, 1, 1);

        appliedVerticalForce = new QDoubleSpinBox(loadProperties);
        appliedVerticalForce->setObjectName(QStringLiteral("appliedVerticalForce"));
        sizePolicy7.setHeightForWidth(appliedVerticalForce->sizePolicy().hasHeightForWidth());
        appliedVerticalForce->setSizePolicy(sizePolicy7);

        gridLayout_14->addWidget(appliedVerticalForce, 4, 1, 1, 1);

        label_23 = new QLabel(loadProperties);
        label_23->setObjectName(QStringLiteral("label_23"));

        gridLayout_14->addWidget(label_23, 6, 2, 1, 1);

        appliedMoment = new QDoubleSpinBox(loadProperties);
        appliedMoment->setObjectName(QStringLiteral("appliedMoment"));
        sizePolicy7.setHeightForWidth(appliedMoment->sizePolicy().hasHeightForWidth());
        appliedMoment->setSizePolicy(sizePolicy7);

        gridLayout_14->addWidget(appliedMoment, 6, 1, 1, 1);

        forceLabelMoment = new QLabel(loadProperties);
        forceLabelMoment->setObjectName(QStringLiteral("forceLabelMoment"));

        gridLayout_14->addWidget(forceLabelMoment, 6, 0, 1, 1);

        appliedHorizontalForce = new QDoubleSpinBox(loadProperties);
        appliedHorizontalForce->setObjectName(QStringLiteral("appliedHorizontalForce"));
        QSizePolicy sizePolicy8(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy8.setHorizontalStretch(2);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(appliedHorizontalForce->sizePolicy().hasHeightForWidth());
        appliedHorizontalForce->setSizePolicy(sizePolicy8);
        appliedHorizontalForce->setDecimals(1);
        appliedHorizontalForce->setMinimum(-10000);
        appliedHorizontalForce->setMaximum(10000);
        appliedHorizontalForce->setSingleStep(100);
        appliedHorizontalForce->setValue(0);

        gridLayout_14->addWidget(appliedHorizontalForce, 2, 1, 1, 1);

        momentSlider = new QSlider(loadProperties);
        momentSlider->setObjectName(QStringLiteral("momentSlider"));
        momentSlider->setOrientation(Qt::Horizontal);

        gridLayout_14->addWidget(momentSlider, 7, 1, 1, 1);


        gridLayout_16->addWidget(loadProperties, 0, 0, 1, 1);

        loadTypesStack->addWidget(page_forceControl);
        page_pushOver = new QWidget();
        page_pushOver->setObjectName(QStringLiteral("page_pushOver"));
        gridLayout_17 = new QGridLayout(page_pushOver);
        gridLayout_17->setSpacing(6);
        gridLayout_17->setContentsMargins(11, 11, 11, 11);
        gridLayout_17->setObjectName(QStringLiteral("gridLayout_17"));
        gridLayout_17->setContentsMargins(0, 0, 0, 0);
        pushOverFrame = new QFrame(page_pushOver);
        pushOverFrame->setObjectName(QStringLiteral("pushOverFrame"));
        pushOverFrame->setFrameShape(QFrame::StyledPanel);
        pushOverFrame->setFrameShadow(QFrame::Raised);
        gridLayout_18 = new QGridLayout(pushOverFrame);
        gridLayout_18->setSpacing(6);
        gridLayout_18->setContentsMargins(11, 11, 11, 11);
        gridLayout_18->setObjectName(QStringLiteral("gridLayout_18"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_18->addItem(verticalSpacer_3, 7, 2, 1, 1);

        label_pullout = new QLabel(pushOverFrame);
        label_pullout->setObjectName(QStringLiteral("label_pullout"));

        gridLayout_18->addWidget(label_pullout, 4, 0, 1, 1);

        label_unitPushover = new QLabel(pushOverFrame);
        label_unitPushover->setObjectName(QStringLiteral("label_unitPushover"));

        gridLayout_18->addWidget(label_unitPushover, 1, 3, 1, 1);

        pushoverDisplacement = new QDoubleSpinBox(pushOverFrame);
        pushoverDisplacement->setObjectName(QStringLiteral("pushoverDisplacement"));

        gridLayout_18->addWidget(pushoverDisplacement, 1, 2, 1, 1);

        pulloutDisplacement = new QDoubleSpinBox(pushOverFrame);
        pulloutDisplacement->setObjectName(QStringLiteral("pulloutDisplacement"));
        QSizePolicy sizePolicy9(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(2);
        sizePolicy9.setHeightForWidth(pulloutDisplacement->sizePolicy().hasHeightForWidth());
        pulloutDisplacement->setSizePolicy(sizePolicy9);

        gridLayout_18->addWidget(pulloutDisplacement, 4, 2, 1, 1);

        label_pushover = new QLabel(pushOverFrame);
        label_pushover->setObjectName(QStringLiteral("label_pushover"));

        gridLayout_18->addWidget(label_pushover, 1, 0, 1, 1);

        pushoverDisplacementSlider = new QSlider(pushOverFrame);
        pushoverDisplacementSlider->setObjectName(QStringLiteral("pushoverDisplacementSlider"));
        pushoverDisplacementSlider->setOrientation(Qt::Horizontal);

        gridLayout_18->addWidget(pushoverDisplacementSlider, 2, 2, 1, 2);

        label_unitPullout = new QLabel(pushOverFrame);
        label_unitPullout->setObjectName(QStringLiteral("label_unitPullout"));

        gridLayout_18->addWidget(label_unitPullout, 4, 3, 1, 1);

        pulloutDisplacementSlider = new QSlider(pushOverFrame);
        pulloutDisplacementSlider->setObjectName(QStringLiteral("pulloutDisplacementSlider"));
        pulloutDisplacementSlider->setOrientation(Qt::Horizontal);

        gridLayout_18->addWidget(pulloutDisplacementSlider, 5, 2, 1, 2);


        gridLayout_17->addWidget(pushOverFrame, 0, 0, 1, 1);

        loadTypesStack->addWidget(page_pushOver);
        page_displacementProfile = new QWidget();
        page_displacementProfile->setObjectName(QStringLiteral("page_displacementProfile"));
        gridLayout_19 = new QGridLayout(page_displacementProfile);
        gridLayout_19->setSpacing(6);
        gridLayout_19->setContentsMargins(11, 11, 11, 11);
        gridLayout_19->setObjectName(QStringLiteral("gridLayout_19"));
        gridLayout_19->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(page_displacementProfile);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_20 = new QGridLayout(frame);
        gridLayout_20->setSpacing(6);
        gridLayout_20->setContentsMargins(11, 11, 11, 11);
        gridLayout_20->setObjectName(QStringLiteral("gridLayout_20"));
        label_BaseDisplacement = new QLabel(frame);
        label_BaseDisplacement->setObjectName(QStringLiteral("label_BaseDisplacement"));

        gridLayout_20->addWidget(label_BaseDisplacement, 6, 0, 1, 1);

        label_Interface23 = new QLabel(frame);
        label_Interface23->setObjectName(QStringLiteral("label_Interface23"));

        gridLayout_20->addWidget(label_Interface23, 4, 0, 1, 1);

        label_SurfaceDisplacement = new QLabel(frame);
        label_SurfaceDisplacement->setObjectName(QStringLiteral("label_SurfaceDisplacement"));

        gridLayout_20->addWidget(label_SurfaceDisplacement, 0, 0, 1, 1);

        Interface23 = new QSpinBox(frame);
        Interface23->setObjectName(QStringLiteral("Interface23"));

        gridLayout_20->addWidget(Interface23, 4, 1, 1, 1);

        label_Interface12 = new QLabel(frame);
        label_Interface12->setObjectName(QStringLiteral("label_Interface12"));

        gridLayout_20->addWidget(label_Interface12, 2, 0, 1, 1);

        surfaceDisplacement = new QDoubleSpinBox(frame);
        surfaceDisplacement->setObjectName(QStringLiteral("surfaceDisplacement"));
        QSizePolicy sizePolicy10(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy10.setHorizontalStretch(0);
        sizePolicy10.setVerticalStretch(0);
        sizePolicy10.setHeightForWidth(surfaceDisplacement->sizePolicy().hasHeightForWidth());
        surfaceDisplacement->setSizePolicy(sizePolicy10);

        gridLayout_20->addWidget(surfaceDisplacement, 0, 1, 1, 1);

        Interface12 = new QSpinBox(frame);
        Interface12->setObjectName(QStringLiteral("Interface12"));

        gridLayout_20->addWidget(Interface12, 2, 1, 1, 1);

        BaseDisplacement = new QSpinBox(frame);
        BaseDisplacement->setObjectName(QStringLiteral("BaseDisplacement"));

        gridLayout_20->addWidget(BaseDisplacement, 6, 1, 1, 1);

        Interface12Slider = new QSlider(frame);
        Interface12Slider->setObjectName(QStringLiteral("Interface12Slider"));
        Interface12Slider->setOrientation(Qt::Horizontal);

        gridLayout_20->addWidget(Interface12Slider, 3, 1, 1, 2);

        label_unitInterface12 = new QLabel(frame);
        label_unitInterface12->setObjectName(QStringLiteral("label_unitInterface12"));

        gridLayout_20->addWidget(label_unitInterface12, 2, 2, 1, 1);

        label_unitSurfaceDisplacement = new QLabel(frame);
        label_unitSurfaceDisplacement->setObjectName(QStringLiteral("label_unitSurfaceDisplacement"));

        gridLayout_20->addWidget(label_unitSurfaceDisplacement, 0, 2, 1, 1);

        surfaceDisplacementSlider = new QSlider(frame);
        surfaceDisplacementSlider->setObjectName(QStringLiteral("surfaceDisplacementSlider"));
        surfaceDisplacementSlider->setOrientation(Qt::Horizontal);

        gridLayout_20->addWidget(surfaceDisplacementSlider, 1, 1, 1, 2);

        label_unitInterface23 = new QLabel(frame);
        label_unitInterface23->setObjectName(QStringLiteral("label_unitInterface23"));

        gridLayout_20->addWidget(label_unitInterface23, 4, 2, 1, 1);

        Interface23Slider = new QSlider(frame);
        Interface23Slider->setObjectName(QStringLiteral("Interface23Slider"));
        Interface23Slider->setOrientation(Qt::Horizontal);

        gridLayout_20->addWidget(Interface23Slider, 5, 1, 1, 2);

        BaseDisplacementSlider = new QSlider(frame);
        BaseDisplacementSlider->setObjectName(QStringLiteral("BaseDisplacementSlider"));
        BaseDisplacementSlider->setOrientation(Qt::Horizontal);

        gridLayout_20->addWidget(BaseDisplacementSlider, 7, 1, 1, 2);

        label_unitBaseDisplacement = new QLabel(frame);
        label_unitBaseDisplacement->setObjectName(QStringLiteral("label_unitBaseDisplacement"));

        gridLayout_20->addWidget(label_unitBaseDisplacement, 6, 2, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_20->addItem(verticalSpacer_4, 8, 1, 1, 1);


        gridLayout_19->addWidget(frame, 0, 0, 1, 1);

        loadTypesStack->addWidget(page_displacementProfile);

        verticalLayout_5->addWidget(loadTypesStack);

        properties->addTab(loadPropertyWidget, QString());

        verticalLayout->addWidget(properties);


        gridLayout_11->addLayout(verticalLayout, 1, 0, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy11(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy11.setHorizontalStretch(3);
        sizePolicy11.setVerticalStretch(1);
        sizePolicy11.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy11);
        tabWidget->setMinimumSize(QSize(0, 0));
        tabWidget->setTabPosition(QTabWidget::East);
        dispTab = new QWidget();
        dispTab->setObjectName(QStringLiteral("dispTab"));
        gridLayout_3 = new QGridLayout(dispTab);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(6, 6, 6, 6);
        tabWidget->addTab(dispTab, QString());
        pulloutTab = new QWidget();
        pulloutTab->setObjectName(QStringLiteral("pulloutTab"));
        gridLayout_15 = new QGridLayout(pulloutTab);
        gridLayout_15->setSpacing(6);
        gridLayout_15->setContentsMargins(11, 11, 11, 11);
        gridLayout_15->setObjectName(QStringLiteral("gridLayout_15"));
        gridLayout_15->setContentsMargins(6, 6, 6, 6);
        tabWidget->addTab(pulloutTab, QString());
        momentTab = new QWidget();
        momentTab->setObjectName(QStringLiteral("momentTab"));
        gridLayout_5 = new QGridLayout(momentTab);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(6, 6, 6, 6);
        tabWidget->addTab(momentTab, QString());
        shearTab = new QWidget();
        shearTab->setObjectName(QStringLiteral("shearTab"));
        gridLayout_4 = new QGridLayout(shearTab);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(6, 6, 6, 6);
        tabWidget->addTab(shearTab, QString());
        axialTab = new QWidget();
        axialTab->setObjectName(QStringLiteral("axialTab"));
        gridLayout_13 = new QGridLayout(axialTab);
        gridLayout_13->setSpacing(6);
        gridLayout_13->setContentsMargins(11, 11, 11, 11);
        gridLayout_13->setObjectName(QStringLiteral("gridLayout_13"));
        gridLayout_13->setContentsMargins(6, 6, 6, 6);
        tabWidget->addTab(axialTab, QString());
        stressTab = new QWidget();
        stressTab->setObjectName(QStringLiteral("stressTab"));
        gridLayout_6 = new QGridLayout(stressTab);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_6->setContentsMargins(6, 6, 6, 6);
        tabWidget->addTab(stressTab, QString());
        pultTab = new QWidget();
        pultTab->setObjectName(QStringLiteral("pultTab"));
        gridLayout_7 = new QGridLayout(pultTab);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        gridLayout_7->setContentsMargins(6, 6, 6, 6);
        tabWidget->addTab(pultTab, QString());
        y50Tab = new QWidget();
        y50Tab->setObjectName(QStringLiteral("y50Tab"));
        gridLayout_8 = new QGridLayout(y50Tab);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        gridLayout_8->setContentsMargins(6, 6, 6, 6);
        tabWidget->addTab(y50Tab, QString());

        gridLayout_11->addWidget(tabWidget, 1, 4, 1, 1);

        tabWidget_system = new QTabWidget(centralWidget);
        tabWidget_system->setObjectName(QStringLiteral("tabWidget_system"));
        QSizePolicy sizePolicy12(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy12.setHorizontalStretch(3);
        sizePolicy12.setVerticalStretch(0);
        sizePolicy12.setHeightForWidth(tabWidget_system->sizePolicy().hasHeightForWidth());
        tabWidget_system->setSizePolicy(sizePolicy12);
        tabWidget_system->setMinimumSize(QSize(0, 0));
        systemTab = new QWidget();
        systemTab->setObjectName(QStringLiteral("systemTab"));
        gridLayout_2 = new QGridLayout(systemTab);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        tabWidget_system->addTab(systemTab, QString());

        gridLayout_11->addWidget(tabWidget_system, 1, 3, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 994, 22));
        menuOpenSeesPile = new QMenu(menuBar);
        menuOpenSeesPile->setObjectName(QStringLiteral("menuOpenSeesPile"));
        menuAnalysis = new QMenu(menuBar);
        menuAnalysis->setObjectName(QStringLiteral("menuAnalysis"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QSizePolicy sizePolicy13(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy13.setHorizontalStretch(0);
        sizePolicy13.setVerticalStretch(0);
        sizePolicy13.setHeightForWidth(mainToolBar->sizePolicy().hasHeightForWidth());
        mainToolBar->setSizePolicy(sizePolicy13);
        mainToolBar->setMinimumSize(QSize(800, 0));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuOpenSeesPile->menuAction());
        menuBar->addAction(menuAnalysis->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuOpenSeesPile->addAction(actionNew);
        menuOpenSeesPile->addAction(action_Open);
        menuOpenSeesPile->addAction(actionSave);
        menuOpenSeesPile->addAction(actionExport_to_OpenSees);
        menuOpenSeesPile->addAction(actionExit);
        menuAnalysis->addAction(actionReset);
        menuAnalysis->addAction(actionFEA_parameters);
        menuHelp->addAction(action_About);
        menuHelp->addAction(actionPreferences);
        menuHelp->addAction(actionProvide_Feedback);
        menuHelp->addAction(actionVersion);
        menuHelp->addAction(actionLicense);

        retranslateUi(MainWindow);

        properties->setCurrentIndex(0);
        loadTypesStack->setCurrentIndex(0);
        tabWidget->setCurrentIndex(7);
        tabWidget_system->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionSave->setText(QApplication::translate("MainWindow", "Save", nullptr));
        actionNew->setText(QApplication::translate("MainWindow", "New", nullptr));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", nullptr));
        actionReset->setText(QApplication::translate("MainWindow", "&Reset", nullptr));
        actionFEA_parameters->setText(QApplication::translate("MainWindow", "FEA &Parameters", nullptr));
        action_Open->setText(QApplication::translate("MainWindow", "&Open", nullptr));
        actionExport_to_OpenSees->setText(QApplication::translate("MainWindow", "Export to OpenSees", nullptr));
        actionLicense_Information->setText(QApplication::translate("MainWindow", "License Information", nullptr));
        actionPreferences->setText(QApplication::translate("MainWindow", "&Preferences", nullptr));
        action_About->setText(QApplication::translate("MainWindow", "&About", nullptr));
        actionProvide_Feedback->setText(QApplication::translate("MainWindow", "Provide Feedback", nullptr));
        actionVersion->setText(QApplication::translate("MainWindow", "Version", nullptr));
        actionLicense->setText(QApplication::translate("MainWindow", "License", nullptr));
        chkBox_include_toe_resistance->setText(QApplication::translate("MainWindow", "Include toe resistance", nullptr));
        chkBox_assume_rigid_cap->setText(QApplication::translate("MainWindow", "Assume rigid connection at pile cap", nullptr));
        lengthLabel->setText(QApplication::translate("MainWindow", "Embedded length:", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "GPa", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "m", nullptr));
        emodulusLabel->setText(QApplication::translate("MainWindow", "Young's modulus (E)", nullptr));
        freeLengthLabel->setText(QApplication::translate("MainWindow", "Length above ground:", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "x (offset):", nullptr));
        diameterLabel->setText(QApplication::translate("MainWindow", "Pile Diameter:", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "m", nullptr));
        pilePropertyLabel->setText(QApplication::translate("MainWindow", "Pile properties", nullptr));
        btn_deletePile->setText(QApplication::translate("MainWindow", "Delete this Pile", nullptr));
        btn_newPile->setText(QApplication::translate("MainWindow", "Add New Pile", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "m", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "m", nullptr));
        label->setText(QApplication::translate("MainWindow", "Pile number:", nullptr));
        properties->setTabText(properties->indexOf(pilePropertiesWidget), QApplication::translate("MainWindow", "Pile(s) Configuration", nullptr));
        label_GWT->setText(QApplication::translate("MainWindow", "Ground water table", nullptr));
        label_depth->setText(QApplication::translate("MainWindow", "Depth below surface:", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "m", nullptr));
        label_Layers->setText(QApplication::translate("MainWindow", "Soil Layers", nullptr));
        chkBox_layer2->setText(QApplication::translate("MainWindow", "#2", nullptr));
        chkBox_layer1->setText(QApplication::translate("MainWindow", "#1", nullptr));
        chkBox_layer3->setText(QApplication::translate("MainWindow", "#3", nullptr));
        labelChangingLayer->setText(QApplication::translate("MainWindow", "Changing layer", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "thickness", nullptr));
        label_15->setText(QApplication::translate("MainWindow", "kN/m3", nullptr));
        label_16->setText(QApplication::translate("MainWindow", "kN/m3", nullptr));
        label_13->setText(QApplication::translate("MainWindow", "friction angle", nullptr));
        label_18->setText(QApplication::translate("MainWindow", "deg", nullptr));
        label_17->setText(QApplication::translate("MainWindow", "m", nullptr));
        label_14->setText(QApplication::translate("MainWindow", "Shear modulus", nullptr));
        label_19->setText(QApplication::translate("MainWindow", "MPa", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "dry unit weight", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "saturated weight", nullptr));
        labelLayerProperties->setText(QApplication::translate("MainWindow", "Layer Properties", nullptr));
        properties->setTabText(properties->indexOf(soilPropertiesWidget), QApplication::translate("MainWindow", "Site Configuration", nullptr));
        label_20->setText(QApplication::translate("MainWindow", "Load control", nullptr));
        forceTypeSelector->setItemText(0, QApplication::translate("MainWindow", "Force control", nullptr));
        forceTypeSelector->setItemText(1, QApplication::translate("MainWindow", "Push-over analysis", nullptr));
        forceTypeSelector->setItemText(2, QApplication::translate("MainWindow", "Soil motion profile", nullptr));

        forcelabelVert->setText(QApplication::translate("MainWindow", "Vertical force:", nullptr));
        label_21->setText(QApplication::translate("MainWindow", "kN", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "kN ", nullptr));
        forceLabelHoriz->setText(QApplication::translate("MainWindow", "Horizontal force:", nullptr));
        label_23->setText(QApplication::translate("MainWindow", "kN.m", nullptr));
        forceLabelMoment->setText(QApplication::translate("MainWindow", "Moment:", nullptr));
        label_pullout->setText(QApplication::translate("MainWindow", "Pull-out\n"
"displacement", nullptr));
        label_unitPushover->setText(QApplication::translate("MainWindow", "m", nullptr));
        label_pushover->setText(QApplication::translate("MainWindow", "Push-over\n"
"displacement", nullptr));
        label_unitPullout->setText(QApplication::translate("MainWindow", "m", nullptr));
        label_BaseDisplacement->setText(QApplication::translate("MainWindow", "Base\n"
"displacement:", nullptr));
        label_Interface23->setText(QApplication::translate("MainWindow", "Interface\n"
"Layers 2 & 3:", nullptr));
        label_SurfaceDisplacement->setText(QApplication::translate("MainWindow", "Surface\n"
"displacement:", nullptr));
        label_Interface12->setText(QApplication::translate("MainWindow", "Interface\n"
"Layers 1 & 2:", nullptr));
        label_unitInterface12->setText(QApplication::translate("MainWindow", "%", nullptr));
        label_unitSurfaceDisplacement->setText(QApplication::translate("MainWindow", "m", nullptr));
        label_unitInterface23->setText(QApplication::translate("MainWindow", "%", nullptr));
        label_unitBaseDisplacement->setText(QApplication::translate("MainWindow", "%", nullptr));
        properties->setTabText(properties->indexOf(loadPropertyWidget), QApplication::translate("MainWindow", "Load Options", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(dispTab), QApplication::translate("MainWindow", "Lateral Disp", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(pulloutTab), QApplication::translate("MainWindow", "Axial Disp", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(momentTab), QApplication::translate("MainWindow", "Moment", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(shearTab), QApplication::translate("MainWindow", "Shear", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(axialTab), QApplication::translate("MainWindow", "Axial", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(stressTab), QApplication::translate("MainWindow", "effective Stress", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(pultTab), QApplication::translate("MainWindow", "p_ult", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(y50Tab), QApplication::translate("MainWindow", "y50", nullptr));
        tabWidget_system->setTabText(tabWidget_system->indexOf(systemTab), QApplication::translate("MainWindow", "System Plot", nullptr));
        menuOpenSeesPile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuAnalysis->setTitle(QApplication::translate("MainWindow", "Analysis", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
