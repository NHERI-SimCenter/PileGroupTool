#ifndef RESULTPLOTQCP_H
#define RESULTPLOTQCP_H

#include "resultplotsuper.h"

class QCustomPlot;

class ResultPlotQCP : public ResultPlotSuper
{
public:
    ResultPlotQCP(QWidget *parent = nullptr);
    ~ResultPlotQCP();
    void plotResults(QVector<QVector<double> *> &x, QVector<QVector<double> *> &y);

protected:
    QCustomPlot *plot;
};

#endif // RESULTPLOTQCP_H
