#ifndef SCTRINPUTGROUP_H
#define SCTRINPUTGROUP_H

#include <QFrame>

namespace Ui {
class SCtrInputGroup;
}

class SCtrInputGroup : public QFrame
{
    Q_OBJECT

public:
    explicit SCtrInputGroup(QWidget *parent = nullptr);
    ~SCtrInputGroup();

signals:
    void valueChanged(int val);

public slots:
    void setValue(int val);
    void setMinimum(int val);
    void setMaximum(int val);
    void setTickInterval(int val);
    void setSingleStep(int val);
    void setRange(int min, int max);

private:
    Ui::SCtrInputGroup *ui;
};

#endif // SCTRINPUTGROUP_H
