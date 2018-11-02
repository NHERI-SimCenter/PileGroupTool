#include "dialogquicktips.h"
#include "ui_dialogquicktips.h"

DialogQuickTips::DialogQuickTips(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogQuickTips)
{
    ui->setupUi(this);
}

DialogQuickTips::~DialogQuickTips()
{
    delete ui;
}
