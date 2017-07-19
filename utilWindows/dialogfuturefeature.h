#ifndef DIALOGFUTUREFEATURE_H
#define DIALOGFUTUREFEATURE_H

#include <QDialog>

namespace Ui {
class DialogFutureFeature;
}

class DialogFutureFeature : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFutureFeature(QWidget *parent = 0);
    ~DialogFutureFeature();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogFutureFeature *ui;
};

#endif // DIALOGFUTUREFEATURE_H
