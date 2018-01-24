#include "resultplotqcp.h"
#include "qcustomplot.h"

ResultPlotQCP::ResultPlotQCP(QWidget *parent) :
    ResultPlotSuper(parent)
{
    plot = new QCustomPlot(this);

}

ResultPlotQCP::~ResultPlotQCP()
{
    delete plot;
}

void ResultPlotQCP::plotResults(QVector<double> &z,
                                QVector<double> &xOffset,
                                QVector<QVector<double> > &x,
                                QVector<QVector<double> > &y)
{
    /*
     * MAKE SURE THE LENGTH OF THE VECTORS IN ALL PLOTS MATCHES THE ACTUAL PILE
     *
     * NO MORE RANGE CHECKING!
     */

    int numPiles = x.size();

    plot->clearPlottables();

    plot->autoAddPlottableToLegend();
    plot->legend->setVisible(true);

    plot->addGraph();
    plot->graph(0)->setData(z,xOffset);
    plot->graph(0)->setPen(QPen(Qt::black));
    plot->graph(0)->removeFromLegend();

    for (int ii=0; ii<numPiles; ii++) {
        QCPCurve *mCurve = new QCPCurve(plot->xAxis, plot->yAxis);
        // mCurve->setData(x[ii].mid(0,numNodePile[ii]),y[ii].mid(0,numNodePile[ii]));
        mCurve->setData(x[ii],y[ii]);
        mCurve->setPen(QPen(LINE_COLOR[ii], 3));
        mCurve->setName(QString("Pile #%1").arg(ii+1));
    }

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    plot->axisRect()->autoMargins();
    plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignBottom);
    plot->rescaleAxes();
    plot->replot();
}

