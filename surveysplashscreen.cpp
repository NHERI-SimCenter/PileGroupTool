#include "surveysplashscreen.h"
#include "ui_surveysplashscreen.h"

surveySplashScreen::surveySplashScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::surveySplashScreen)
{
    ui->setupUi(this);

    /* populate information and survey text info */
    QString HTMLtext = "This is the splash screen\
                        to demonstrate feature\
                        <a href=\"http://www.uw.edu\">survey hyperlink</a>\
                        and more info.";
    ui->surveyText->insertHtml(HTMLtext);
}

surveySplashScreen::~surveySplashScreen()
{
    delete ui;
}

void surveySplashScreen::on_pushButton_OK_clicked()
{
    this->close();
}
