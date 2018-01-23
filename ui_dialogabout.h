/********************************************************************************
** Form generated from reading UI file 'dialogabout.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGABOUT_H
#define UI_DIALOGABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DialogAbout
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QDialog *DialogAbout)
    {
        if (DialogAbout->objectName().isEmpty())
            DialogAbout->setObjectName(QStringLiteral("DialogAbout"));
        DialogAbout->resize(476, 433);
        gridLayout = new QGridLayout(DialogAbout);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        buttonBox = new QDialogButtonBox(DialogAbout);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setLayoutDirection(Qt::LeftToRight);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 2, 0, 1, 1);

        plainTextEdit = new QPlainTextEdit(DialogAbout);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));

        gridLayout->addWidget(plainTextEdit, 0, 0, 1, 1);


        retranslateUi(DialogAbout);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogAbout, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogAbout, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogAbout);
    } // setupUi

    void retranslateUi(QDialog *DialogAbout)
    {
        DialogAbout->setWindowTitle(QApplication::translate("DialogAbout", "Dialog", nullptr));
        plainTextEdit->setPlainText(QApplication::translate("DialogAbout", "This is the SimCenter Educational Pile Group Tool. \n"
"\n"
"Please be aware that this software is not intended to be used in any commercial analysis and/or design.  All contributors of NHERI SimCenter are free of responsibility and liability resulting from the use of this software. Use of this software is at your own risk. \n"
"\n"
"We, the SimCenter software designers, are asking you to share your suggestions on how to improve this educational tool by reporting issues at https://github.com/NHERI-SimCenter/QtPile. \n"
"", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogAbout: public Ui_DialogAbout {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGABOUT_H
