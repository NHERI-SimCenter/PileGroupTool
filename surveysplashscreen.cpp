#include "surveysplashscreen.h"
#include "ui_surveysplashscreen.h"

surveySplashScreen::surveySplashScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::surveySplashScreen)
{
    ui->setupUi(this);

    /* populate information and survey text info */
    QString HTMLtext = "\
            Dear user,\
            <p>\
            You are about to explore an alpha release of the SimCenter Educational Pile Tool.\
            This version of the software is to provide an initial impression of features and functionality, \
            though several intended features are under development.\
            Future versions will incorporate features such as pile groups and more complex layer structure.\
            <p>\
            Please be aware that this software is not intended to be used in any commercial\
            analysis and/or design. The software may not be redistributed other than through \
            <a href=\"https://www.designsafe-ci.org/\">https://www.designsafe-ci.org</a>.\
            All contributors of NHERI SimCenter are free of responsibility and liability resulting\
            from the use of this software.  Use of this software is at your own risk.\
            <p>\
            We, the SimCenter software designers, are asking you to share your suggestions on how \
            to improve this educational tool at \
            <a href=\"http://uw.edu\">http://uw.edu</a>.  \
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
