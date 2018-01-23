/********************************************************************************
** Form generated from reading UI file 'materialdbinterface.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATERIALDBINTERFACE_H
#define UI_MATERIALDBINTERFACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MaterialDBInterface
{
public:
    QGridLayout *gridLayout_3;
    QComboBox *cb_selectDB;
    QListWidget *lst_materialSelection;
    QStackedWidget *stackedWidget;
    QWidget *tab_knownMaterial;
    QGridLayout *gridLayout;
    QTableWidget *tbl_materialProperties;
    QWidget *tab_newMaterial;
    QGridLayout *gridLayout_2;
    QTableWidget *tbl_materialProperties_2;
    QSplitter *splitter;
    QLabel *label;
    QLineEdit *lned_materialName;
    QLabel *label_2;
    QLineEdit *lned_numProperties;
    QSplitter *splitter_2;
    QPushButton *pb_reset;
    QPushButton *pb_commit;

    void setupUi(QWidget *MaterialDBInterface)
    {
        if (MaterialDBInterface->objectName().isEmpty())
            MaterialDBInterface->setObjectName(QStringLiteral("MaterialDBInterface"));
        MaterialDBInterface->resize(529, 540);
        gridLayout_3 = new QGridLayout(MaterialDBInterface);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        cb_selectDB = new QComboBox(MaterialDBInterface);
        cb_selectDB->addItem(QString());
        cb_selectDB->addItem(QString());
        cb_selectDB->addItem(QString());
        cb_selectDB->setObjectName(QStringLiteral("cb_selectDB"));

        gridLayout_3->addWidget(cb_selectDB, 0, 0, 1, 1);

        lst_materialSelection = new QListWidget(MaterialDBInterface);
        new QListWidgetItem(lst_materialSelection);
        new QListWidgetItem(lst_materialSelection);
        new QListWidgetItem(lst_materialSelection);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(lst_materialSelection);
        __qlistwidgetitem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        lst_materialSelection->setObjectName(QStringLiteral("lst_materialSelection"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(lst_materialSelection->sizePolicy().hasHeightForWidth());
        lst_materialSelection->setSizePolicy(sizePolicy);

        gridLayout_3->addWidget(lst_materialSelection, 1, 0, 1, 1);

        stackedWidget = new QStackedWidget(MaterialDBInterface);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(stackedWidget->sizePolicy().hasHeightForWidth());
        stackedWidget->setSizePolicy(sizePolicy1);
        tab_knownMaterial = new QWidget();
        tab_knownMaterial->setObjectName(QStringLiteral("tab_knownMaterial"));
        gridLayout = new QGridLayout(tab_knownMaterial);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tbl_materialProperties = new QTableWidget(tab_knownMaterial);
        if (tbl_materialProperties->columnCount() < 4)
            tbl_materialProperties->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tbl_materialProperties->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tbl_materialProperties->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tbl_materialProperties->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tbl_materialProperties->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        if (tbl_materialProperties->rowCount() < 4)
            tbl_materialProperties->setRowCount(4);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tbl_materialProperties->setVerticalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tbl_materialProperties->setVerticalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tbl_materialProperties->setVerticalHeaderItem(2, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tbl_materialProperties->setVerticalHeaderItem(3, __qtablewidgetitem7);
        tbl_materialProperties->setObjectName(QStringLiteral("tbl_materialProperties"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(1);
        sizePolicy2.setHeightForWidth(tbl_materialProperties->sizePolicy().hasHeightForWidth());
        tbl_materialProperties->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(tbl_materialProperties, 0, 0, 1, 1);

        stackedWidget->addWidget(tab_knownMaterial);
        tab_newMaterial = new QWidget();
        tab_newMaterial->setObjectName(QStringLiteral("tab_newMaterial"));
        gridLayout_2 = new QGridLayout(tab_newMaterial);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        tbl_materialProperties_2 = new QTableWidget(tab_newMaterial);
        if (tbl_materialProperties_2->columnCount() < 4)
            tbl_materialProperties_2->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tbl_materialProperties_2->setHorizontalHeaderItem(0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tbl_materialProperties_2->setHorizontalHeaderItem(1, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tbl_materialProperties_2->setHorizontalHeaderItem(2, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tbl_materialProperties_2->setHorizontalHeaderItem(3, __qtablewidgetitem11);
        if (tbl_materialProperties_2->rowCount() < 4)
            tbl_materialProperties_2->setRowCount(4);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tbl_materialProperties_2->setVerticalHeaderItem(0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tbl_materialProperties_2->setVerticalHeaderItem(1, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tbl_materialProperties_2->setVerticalHeaderItem(2, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tbl_materialProperties_2->setVerticalHeaderItem(3, __qtablewidgetitem15);
        tbl_materialProperties_2->setObjectName(QStringLiteral("tbl_materialProperties_2"));
        sizePolicy2.setHeightForWidth(tbl_materialProperties_2->sizePolicy().hasHeightForWidth());
        tbl_materialProperties_2->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(tbl_materialProperties_2, 1, 0, 1, 1);

        splitter = new QSplitter(tab_newMaterial);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        label = new QLabel(splitter);
        label->setObjectName(QStringLiteral("label"));
        splitter->addWidget(label);
        lned_materialName = new QLineEdit(splitter);
        lned_materialName->setObjectName(QStringLiteral("lned_materialName"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(3);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lned_materialName->sizePolicy().hasHeightForWidth());
        lned_materialName->setSizePolicy(sizePolicy3);
        splitter->addWidget(lned_materialName);
        label_2 = new QLabel(splitter);
        label_2->setObjectName(QStringLiteral("label_2"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy4);
        splitter->addWidget(label_2);
        lned_numProperties = new QLineEdit(splitter);
        lned_numProperties->setObjectName(QStringLiteral("lned_numProperties"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(1);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(lned_numProperties->sizePolicy().hasHeightForWidth());
        lned_numProperties->setSizePolicy(sizePolicy5);
        lned_numProperties->setMinimumSize(QSize(35, 0));
        lned_numProperties->setMaximumSize(QSize(35, 16777215));
        splitter->addWidget(lned_numProperties);

        gridLayout_2->addWidget(splitter, 0, 0, 1, 1);

        splitter_2 = new QSplitter(tab_newMaterial);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        pb_reset = new QPushButton(splitter_2);
        pb_reset->setObjectName(QStringLiteral("pb_reset"));
        splitter_2->addWidget(pb_reset);
        pb_commit = new QPushButton(splitter_2);
        pb_commit->setObjectName(QStringLiteral("pb_commit"));
        splitter_2->addWidget(pb_commit);

        gridLayout_2->addWidget(splitter_2, 2, 0, 1, 1);

        stackedWidget->addWidget(tab_newMaterial);

        gridLayout_3->addWidget(stackedWidget, 2, 0, 1, 1);


        retranslateUi(MaterialDBInterface);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MaterialDBInterface);
    } // setupUi

    void retranslateUi(QWidget *MaterialDBInterface)
    {
        MaterialDBInterface->setWindowTitle(QApplication::translate("MaterialDBInterface", "Form", nullptr));
        cb_selectDB->setItemText(0, QApplication::translate("MaterialDBInterface", "-- select database --", nullptr));
        cb_selectDB->setItemText(1, QApplication::translate("MaterialDBInterface", "LocalDB (default)", nullptr));
        cb_selectDB->setItemText(2, QApplication::translate("MaterialDBInterface", "SimCenter GeoDB", nullptr));


        const bool __sortingEnabled = lst_materialSelection->isSortingEnabled();
        lst_materialSelection->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = lst_materialSelection->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("MaterialDBInterface", "Create new material definition", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = lst_materialSelection->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("MaterialDBInterface", "material 1", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = lst_materialSelection->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("MaterialDBInterface", "material 2", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = lst_materialSelection->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("MaterialDBInterface", "material 3", nullptr));
        lst_materialSelection->setSortingEnabled(__sortingEnabled);

        QTableWidgetItem *___qtablewidgetitem = tbl_materialProperties->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MaterialDBInterface", "property", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tbl_materialProperties->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MaterialDBInterface", "value", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tbl_materialProperties->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MaterialDBInterface", "unit", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tbl_materialProperties->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MaterialDBInterface", "description", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tbl_materialProperties->verticalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("MaterialDBInterface", "prop 1", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tbl_materialProperties->verticalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("MaterialDBInterface", "prop 2", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tbl_materialProperties->verticalHeaderItem(2);
        ___qtablewidgetitem6->setText(QApplication::translate("MaterialDBInterface", "prop 3", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tbl_materialProperties->verticalHeaderItem(3);
        ___qtablewidgetitem7->setText(QApplication::translate("MaterialDBInterface", "prop 4", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tbl_materialProperties_2->horizontalHeaderItem(0);
        ___qtablewidgetitem8->setText(QApplication::translate("MaterialDBInterface", "property", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tbl_materialProperties_2->horizontalHeaderItem(1);
        ___qtablewidgetitem9->setText(QApplication::translate("MaterialDBInterface", "value", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tbl_materialProperties_2->horizontalHeaderItem(2);
        ___qtablewidgetitem10->setText(QApplication::translate("MaterialDBInterface", "unit", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tbl_materialProperties_2->horizontalHeaderItem(3);
        ___qtablewidgetitem11->setText(QApplication::translate("MaterialDBInterface", "description", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tbl_materialProperties_2->verticalHeaderItem(0);
        ___qtablewidgetitem12->setText(QApplication::translate("MaterialDBInterface", "prop 1", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tbl_materialProperties_2->verticalHeaderItem(1);
        ___qtablewidgetitem13->setText(QApplication::translate("MaterialDBInterface", "prop 2", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tbl_materialProperties_2->verticalHeaderItem(2);
        ___qtablewidgetitem14->setText(QApplication::translate("MaterialDBInterface", "prop 3", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tbl_materialProperties_2->verticalHeaderItem(3);
        ___qtablewidgetitem15->setText(QApplication::translate("MaterialDBInterface", "prop 4", nullptr));
        label->setText(QApplication::translate("MaterialDBInterface", "name:", nullptr));
        label_2->setText(QApplication::translate("MaterialDBInterface", "# of properties:", nullptr));
        pb_reset->setText(QApplication::translate("MaterialDBInterface", "reset", nullptr));
        pb_commit->setText(QApplication::translate("MaterialDBInterface", "commit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MaterialDBInterface: public Ui_MaterialDBInterface {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATERIALDBINTERFACE_H
