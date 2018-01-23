/********************************************************************************
** Form generated from reading UI file 'dialogfuturefeature.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGFUTUREFEATURE_H
#define UI_DIALOGFUTUREFEATURE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogFutureFeature
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogFutureFeature)
    {
        if (DialogFutureFeature->objectName().isEmpty())
            DialogFutureFeature->setObjectName(QStringLiteral("DialogFutureFeature"));
        DialogFutureFeature->resize(380, 143);
        verticalLayout = new QVBoxLayout(DialogFutureFeature);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        textEdit = new QTextEdit(DialogFutureFeature);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        verticalLayout->addWidget(textEdit);

        buttonBox = new QDialogButtonBox(DialogFutureFeature);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogFutureFeature);

        QMetaObject::connectSlotsByName(DialogFutureFeature);
    } // setupUi

    void retranslateUi(QDialog *DialogFutureFeature)
    {
        DialogFutureFeature->setWindowTitle(QApplication::translate("DialogFutureFeature", "Dialog", nullptr));
        textEdit->setHtml(QApplication::translate("DialogFutureFeature", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'.SF NS Text'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">This feature is not available in this release but is scheduled for implementation in a future release. Please come back and check for updates if you are interested in this feature.</p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogFutureFeature: public Ui_DialogFutureFeature {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGFUTUREFEATURE_H
