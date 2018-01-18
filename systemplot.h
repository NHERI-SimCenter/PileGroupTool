#ifndef SYSTEMPLOT_H
#define SYSTEMPLOT_H

#include <QWidget>

namespace Ui {
class SystemPlot;
}

class SystemPlot : public QWidget
{
    Q_OBJECT

public:
    explicit SystemPlot(QWidget *parent = 0);
    ~SystemPlot();

private:
    Ui::SystemPlot *ui;
};

#endif // SYSTEMPLOT_H
