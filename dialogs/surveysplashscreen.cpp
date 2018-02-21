#include "surveysplashscreen.h"
#include "ui_surveysplashscreen.h"

surveySplashScreen::surveySplashScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::surveySplashScreen)
{
    ui->setupUi(this);

    /* populate information and survey text info */
    QString HTMLtext = "\
            You are about to explore the SimCenter Educational Pile Tool.\
            <p>\
            Please be aware that this software is not intended to be used in any commercial\
            analysis and/or design. The software may not be redistributed other than through \
            <a href=\"https://www.designsafe-ci.org/\">https://www.designsafe-ci.org</a>.\
            All contributors of NHERI SimCenter are free of responsibility and liability resulting\
            from the use of this software.  Use of this software is at your own risk.\
            <p>\
            We, the SimCenter software designers, are asking you to share your suggestions on how to improve \
            this educational tool by reporting issues at \
            <a target=\"extern\" href=\"https://github.com/NHERI-SimCenter/QtPile\">https://github.com/NHERI-SimCenter/QtPile</a>.  \
            <p>\
            Your SimCenter team";
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
