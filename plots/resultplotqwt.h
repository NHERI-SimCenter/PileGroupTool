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
    void plotResults(QVector<QVector<double> *> &x, QVector<QVector<double> *> &y);

protected:
    QwtPlot *plot;
    QwtPlotGrid *grid;
};

#endif // RESULTPLOTQWT_H
