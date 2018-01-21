#ifndef SYSTEMPLOTQCP_H
#define SYSTEMPLOTQCP_H

#include <QWidget>
#include "pilegrouptool_parameters.h"
#include "systemplotsuper.h"

namespace Ui {
class SystemPlotQCP;
}

class SystemPlotQCP : public SystemPlotSuper
{
    Q_OBJECT

public:
    explicit SystemPlotQCP(QWidget *parent = 0);
    ~SystemPlotQCP();
    void refresh();

private:

protected:
    QCustomPlot *plot;
};

#endif // SYSTEMPLOTQCP_H
