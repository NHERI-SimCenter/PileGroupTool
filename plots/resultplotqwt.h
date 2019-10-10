#ifndef RESULTPLOTQWT_H
#define RESULTPLOTQWT_H

#include "resultplotsuper.h"

class QwtPlot;
class QwtPlotGrid;

class ResultPlotQwt : public ResultPlotSuper
{
public:
    ResultPlotQwt(QWidget *parent = nullptr);
    ~ResultPlotQwt();
    void refresh(void);

protected:
    QwtPlot     *plot;
    QwtPlotGrid *grid;
};

#endif // RESULTPLOTQWT_H
