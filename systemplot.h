#ifndef SYSTEMPLOT_H
#define SYSTEMPLOT_H

#include <QWidget>
#include "pilegrouptool_parameters.h"
#include "systemplotsuper.h"

namespace Ui {
class SystemPlot;
}

class SystemPlot : public SystemPlotSuper
{
    Q_OBJECT

public:
    explicit SystemPlot(QWidget *parent = 0);
    ~SystemPlot();
    void refresh();

private:

protected:
    QCustomPlot *plot;
};

#endif // SYSTEMPLOT_H
