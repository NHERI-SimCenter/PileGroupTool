/********************************************************************************
** Form generated from reading UI file 'surveysplashscreen.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SURVEYSPLASHSCREEN_H
#define UI_SURVEYSPLASHSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_surveySplashScreen
{
public:
    QVBoxLayout *verticalLayout;
    QTextBrowser *surveyText;
    QPushButton *pushButton_OK;

    void setupUi(QDialog *surveySplashScreen)
    {
        if (surveySplashScreen->objectName().isEmpty())
            surveySplashScreen->setObjectName(QStringLiteral("surveySplashScreen"));
        surveySplashScreen->resize(473, 385);
        verticalLayout = new QVBoxLayout(surveySplashScreen);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        surveyText = new QTextBrowser(surveySplashScreen);
        surveyText->setObjectName(QStringLiteral("surveyText"));

        verticalLayout->addWidget(surveyText);

        pushButton_OK = new QPushButton(surveySplashScreen);
        pushButton_OK->setObjectName(QStringLiteral("pushButton_OK"));

        verticalLayout->addWidget(pushButton_OK);


        retranslateUi(surveySplashScreen);

        QMetaObject::connectSlotsByName(surveySplashScreen);
    } // setupUi

    void retranslateUi(QDialog *surveySplashScreen)
    {
        surveySplashScreen->setWindowTitle(QApplication::translate("surveySplashScreen", "Dialog", nullptr));
        pushButton_OK->setText(QApplication::translate("surveySplashScreen", "Run the Application", nullptr));
    } // retranslateUi

};

namespace Ui {
    class surveySplashScreen: public Ui_surveySplashScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SURVEYSPLASHSCREEN_H
