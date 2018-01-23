/********************************************************************************
** Form generated from reading UI file 'dialogpreferences.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPREFERENCES_H
#define UI_DIALOGPREFERENCES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogPreferences
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_3;
    QGridLayout *gridLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_8;
    QRadioButton *rbtn_useQCP;
    QRadioButton *rbtn_useQwt;
    QSpacerItem *verticalSpacer;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_9;
    QRadioButton *rbtn_OpenSeesInt;
    QRadioButton *rbtn_OpenSeesExt;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QCheckBox *chkBx_displacement;
    QCheckBox *chkBx_pullOut;
    QCheckBox *chkBx_moment;
    QCheckBox *chkBx_shear;
    QCheckBox *chkBx_axialForce;
    QCheckBox *chkBx_stress;
    QCheckBox *chkBx_pu;
    QCheckBox *chkBx_y50;
    QWidget *tab_2;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label_7;
    QSpinBox *maxNumberOfPiles;
    QLabel *label_4;
    QSpinBox *minElementsPerLayer;
    QLabel *label_6;
    QSpinBox *maxElementsPerLayer;
    QLabel *label_3;
    QSpinBox *elementsAboveGround;
    QLabel *label_5;
    QDoubleSpinBox *iterationTolerance;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogPreferences)
    {
        if (DialogPreferences->objectName().isEmpty())
            DialogPreferences->setObjectName(QStringLiteral("DialogPreferences"));
        DialogPreferences->resize(400, 418);
        verticalLayout = new QVBoxLayout(DialogPreferences);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(DialogPreferences);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        gridLayout_2 = new QGridLayout(tab_3);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(6, 6, 6, 6);
        frame = new QFrame(tab_3);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setMinimumSize(QSize(0, 20));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_6 = new QVBoxLayout(frame);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        label_8 = new QLabel(frame);
        label_8->setObjectName(QStringLiteral("label_8"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_8->setFont(font);

        verticalLayout_6->addWidget(label_8);

        rbtn_useQCP = new QRadioButton(frame);
        rbtn_useQCP->setObjectName(QStringLiteral("rbtn_useQCP"));
        rbtn_useQCP->setChecked(true);

        verticalLayout_6->addWidget(rbtn_useQCP);

        rbtn_useQwt = new QRadioButton(frame);
        rbtn_useQwt->setObjectName(QStringLiteral("rbtn_useQwt"));

        verticalLayout_6->addWidget(rbtn_useQwt);


        gridLayout_2->addWidget(frame, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 2, 0, 1, 1);

        frame_2 = new QFrame(tab_3);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setMinimumSize(QSize(0, 20));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_5 = new QVBoxLayout(frame_2);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_9 = new QLabel(frame_2);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setFont(font);

        verticalLayout_5->addWidget(label_9);

        rbtn_OpenSeesInt = new QRadioButton(frame_2);
        rbtn_OpenSeesInt->setObjectName(QStringLiteral("rbtn_OpenSeesInt"));
        rbtn_OpenSeesInt->setCheckable(true);
        rbtn_OpenSeesInt->setChecked(true);

        verticalLayout_5->addWidget(rbtn_OpenSeesInt);

        rbtn_OpenSeesExt = new QRadioButton(frame_2);
        rbtn_OpenSeesExt->setObjectName(QStringLiteral("rbtn_OpenSeesExt"));
        rbtn_OpenSeesExt->setCheckable(false);

        verticalLayout_5->addWidget(rbtn_OpenSeesExt);


        gridLayout_2->addWidget(frame_2, 1, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(0, 20));
        label->setMaximumSize(QSize(16777215, 30));
        label->setFont(font);

        verticalLayout_2->addWidget(label);

        chkBx_displacement = new QCheckBox(tab);
        chkBx_displacement->setObjectName(QStringLiteral("chkBx_displacement"));
        chkBx_displacement->setEnabled(true);

        verticalLayout_2->addWidget(chkBx_displacement);

        chkBx_pullOut = new QCheckBox(tab);
        chkBx_pullOut->setObjectName(QStringLiteral("chkBx_pullOut"));

        verticalLayout_2->addWidget(chkBx_pullOut);

        chkBx_moment = new QCheckBox(tab);
        chkBx_moment->setObjectName(QStringLiteral("chkBx_moment"));
        chkBx_moment->setEnabled(true);

        verticalLayout_2->addWidget(chkBx_moment);

        chkBx_shear = new QCheckBox(tab);
        chkBx_shear->setObjectName(QStringLiteral("chkBx_shear"));

        verticalLayout_2->addWidget(chkBx_shear);

        chkBx_axialForce = new QCheckBox(tab);
        chkBx_axialForce->setObjectName(QStringLiteral("chkBx_axialForce"));

        verticalLayout_2->addWidget(chkBx_axialForce);

        chkBx_stress = new QCheckBox(tab);
        chkBx_stress->setObjectName(QStringLiteral("chkBx_stress"));

        verticalLayout_2->addWidget(chkBx_stress);

        chkBx_pu = new QCheckBox(tab);
        chkBx_pu->setObjectName(QStringLiteral("chkBx_pu"));

        verticalLayout_2->addWidget(chkBx_pu);

        chkBx_y50 = new QCheckBox(tab);
        chkBx_y50->setObjectName(QStringLiteral("chkBx_y50"));

        verticalLayout_2->addWidget(chkBx_y50);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout = new QGridLayout(tab_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_2 = new QLabel(tab_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(0, 20));
        label_2->setMaximumSize(QSize(16777215, 30));
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 0, 0, 1, 2);

        label_7 = new QLabel(tab_2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setEnabled(false);

        gridLayout->addWidget(label_7, 1, 0, 1, 1);

        maxNumberOfPiles = new QSpinBox(tab_2);
        maxNumberOfPiles->setObjectName(QStringLiteral("maxNumberOfPiles"));
        maxNumberOfPiles->setEnabled(false);

        gridLayout->addWidget(maxNumberOfPiles, 1, 1, 1, 1);

        label_4 = new QLabel(tab_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setEnabled(false);

        gridLayout->addWidget(label_4, 2, 0, 1, 1);

        minElementsPerLayer = new QSpinBox(tab_2);
        minElementsPerLayer->setObjectName(QStringLiteral("minElementsPerLayer"));
        minElementsPerLayer->setEnabled(false);

        gridLayout->addWidget(minElementsPerLayer, 2, 1, 1, 1);

        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setEnabled(false);

        gridLayout->addWidget(label_6, 3, 0, 1, 1);

        maxElementsPerLayer = new QSpinBox(tab_2);
        maxElementsPerLayer->setObjectName(QStringLiteral("maxElementsPerLayer"));
        maxElementsPerLayer->setEnabled(false);

        gridLayout->addWidget(maxElementsPerLayer, 3, 1, 1, 1);

        label_3 = new QLabel(tab_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setEnabled(false);

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        elementsAboveGround = new QSpinBox(tab_2);
        elementsAboveGround->setObjectName(QStringLiteral("elementsAboveGround"));
        elementsAboveGround->setEnabled(false);

        gridLayout->addWidget(elementsAboveGround, 4, 1, 1, 1);

        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setEnabled(false);

        gridLayout->addWidget(label_5, 5, 0, 1, 1);

        iterationTolerance = new QDoubleSpinBox(tab_2);
        iterationTolerance->setObjectName(QStringLiteral("iterationTolerance"));
        iterationTolerance->setEnabled(false);

        gridLayout->addWidget(iterationTolerance, 5, 1, 1, 1);

        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(DialogPreferences);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Reset);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogPreferences);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogPreferences, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogPreferences, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DialogPreferences);
    } // setupUi

    void retranslateUi(QDialog *DialogPreferences)
    {
        DialogPreferences->setWindowTitle(QApplication::translate("DialogPreferences", "Dialog", nullptr));
        label_8->setText(QApplication::translate("DialogPreferences", "Graphics library", nullptr));
        rbtn_useQCP->setText(QApplication::translate("DialogPreferences", "QCustomPlot (GPL3)", nullptr));
        rbtn_useQwt->setText(QApplication::translate("DialogPreferences", "Qwt (BSD license)", nullptr));
        label_9->setText(QApplication::translate("DialogPreferences", "Finite Element Analyzer", nullptr));
        rbtn_OpenSeesInt->setText(QApplication::translate("DialogPreferences", "OpenSees (internal)", nullptr));
        rbtn_OpenSeesExt->setText(QApplication::translate("DialogPreferences", "OpenSees (external) ... not available", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("DialogPreferences", "General", nullptr));
        label->setText(QApplication::translate("DialogPreferences", "Display Graphics", nullptr));
        chkBx_displacement->setText(QApplication::translate("DialogPreferences", "Lateral displacement", nullptr));
        chkBx_pullOut->setText(QApplication::translate("DialogPreferences", "Axial displacement", nullptr));
        chkBx_moment->setText(QApplication::translate("DialogPreferences", "Moment diagram", nullptr));
        chkBx_shear->setText(QApplication::translate("DialogPreferences", "Shear diagram", nullptr));
        chkBx_axialForce->setText(QApplication::translate("DialogPreferences", "Axial force diagram", nullptr));
        chkBx_stress->setText(QApplication::translate("DialogPreferences", "Vertical stress diagram", nullptr));
        chkBx_pu->setText(QApplication::translate("DialogPreferences", "p ultimate graph", nullptr));
        chkBx_y50->setText(QApplication::translate("DialogPreferences", "compliance (y50) graph", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("DialogPreferences", "Results", nullptr));
        label_2->setText(QApplication::translate("DialogPreferences", "OpenSees options (information only)", nullptr));
        label_7->setText(QApplication::translate("DialogPreferences", "Max number of piles:", nullptr));
        label_4->setText(QApplication::translate("DialogPreferences", "Min elements per layer:", nullptr));
        label_6->setText(QApplication::translate("DialogPreferences", "Max elements per layer:", nullptr));
        label_3->setText(QApplication::translate("DialogPreferences", "Elements above ground:", nullptr));
        label_5->setText(QApplication::translate("DialogPreferences", "Iteration tolerance:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("DialogPreferences", "FEA parameters", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogPreferences: public Ui_DialogPreferences {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPREFERENCES_H
