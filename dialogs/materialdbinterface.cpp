#include "materialdbinterface.h"
#include "ui_materialdbinterface.h"

MaterialDBInterface::MaterialDBInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaterialDBInterface)
{
    ui->setupUi(this);
}

MaterialDBInterface::~MaterialDBInterface()
{
    delete ui;
}

void MaterialDBInterface::on_cb_selectDB_activated(const QString &arg1)
{

}

void MaterialDBInterface::on_lst_materialSelection_clicked(const QModelIndex &index)
{

}

void MaterialDBInterface::on_pb_reset_clicked()
{
    // reset information in data fields

}

void MaterialDBInterface::on_pb_commit_clicked()
{
    // collect information from data fields
}

void MaterialDBInterface::on_lned_numProperties_returnPressed()
{

}
