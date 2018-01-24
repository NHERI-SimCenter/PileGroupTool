#ifndef SYSTEMPLOTQWT_H
#define SYSTEMPLOTQWT_H

#include <QWidget>
#include "pilegrouptool_parameters.h"
#include "systemplotsuper.h"

class QwtPlot;


namespace Ui {
class SystemPlotQwt;
}

class SystemPlotQwt : public SystemPlotSuper
{
    Q_OBJECT

public:
    explicit SystemPlotQwt(QWidget *parent = 0);
    ~SystemPlotQwt();

    void refresh();

private:

signals:
    void on_pileSelected(int );
    void on_soilLayerSelected(int );
    void on_groundWaterSelected(void);

protected:
    QwtPlot *plot;

};

#endif // SYSTEMPLOTQWT_H
