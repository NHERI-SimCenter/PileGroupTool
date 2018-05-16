#ifndef RESULTPLOTQCP_H
#define RESULTPLOTQCP_H

#include "resultplotsuper.h"

class QCustomPlot;

class ResultPlotQCP : public ResultPlotSuper
{
public:
    ResultPlotQCP(QWidget *parent = nullptr);
    ~ResultPlotQCP();
    void refresh(void);

protected:
    QCustomPlot *plot;
};

#endif // RESULTPLOTQCP_H
