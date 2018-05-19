#include "dialogabout.h"
#include "ui_dialogabout.h"
#include <QApplication>
#include <QFile>

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    QFile file(":/help/About/PGTabout.html");
    if (file.open(QFile::ReadOnly))
    {
        ui->textBrowser->setHtml(file.readAll());
    }

}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::on_btn_backToTop_clicked()
{
    ui->textBrowser->setSource(QUrl("#top"));
}
