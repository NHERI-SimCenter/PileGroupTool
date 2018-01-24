#include "resultplotqwt.h"
#include "qwt_plot.h"
#include <QGridLayout>

ResultPlotQwt::ResultPlotQwt(QWidget *parent) :
    ResultPlotSuper(parent)
{
    plot = new QwtPlot(this);

    QGridLayout *lyt = new QGridLayout(this);
    lyt->addWidget(plot,0,0);
    lyt->setMargin(0);
    this->setLayout(lyt);

}

ResultPlotQwt::~ResultPlotQwt()
{
    delete plot;
}

void ResultPlotQwt::plotResults(QVector<double> &z,
                                QVector<double> &xOffset,
                                QVector<QVector<double> > &x,
                                QVector<QVector<double> > &y)
{
#if 0
    plot->clearPlottables();

    plot->autoAddPlottableToLegend();
    plot->legend->setVisible(true);

    plot->addGraph();
    plot->graph(0)->setData(z,xOffset);
    plot->graph(0)->setPen(QPen(Qt::black));
    plot->graph(0)->removeFromLegend();

    for (int ii=0; ii<numPiles; ii++) {
        QCPCurve *mCurve = new QCPCurve(plot->xAxis, plot->yAxis);
        mCurve->setData(x[ii].mid(0,numNodePile[ii]),y[ii].mid(0,numNodePile[ii]));
        mCurve->setPen(QPen(LINE_COLOR[ii], 3));
        mCurve->setName(QString("Pile #%1").arg(ii+1));
        //plot->addPlottable(mCurve);
    }

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    plot->axisRect()->autoMargins();
    plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignBottom);
    plot->rescaleAxes();
    plot->replot();
#endif
}

