#ifndef DIALOGQUICKTIPS_H
#define DIALOGQUICKTIPS_H

#include <QDialog>

namespace Ui {
class DialogQuickTips;
}

class DialogQuickTips : public QDialog
{
    Q_OBJECT

public:
    explicit DialogQuickTips(QWidget *parent = nullptr);
    ~DialogQuickTips();

private:
    Ui::DialogQuickTips *ui;
};

#endif // DIALOGQUICKTIPS_H
