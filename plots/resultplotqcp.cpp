/*!
 * \class ResultPlotQCP
 * \brief The ResultPlotQwt class implements the QCP library interface for ResultPlotSuper.
 * \since 2.0
 * \ingroup Plotting
 *
 * This class provides library-dependent plotting for QCustomPlot (QCP).
 *
 *  \sa ResultPlotQwt
 */


#include "resultplotqcp.h"
#include "qcp/qcustomplot.h"

ResultPlotQCP::ResultPlotQCP(QWidget *parent) :
    ResultPlotSuper(parent)
{
    plot = new QCustomPlot(this);

    QGridLayout *lyt = new QGridLayout(this);
    lyt->addWidget(plot,0,0);
    lyt->setMargin(0);
    this->setLayout(lyt);

}

ResultPlotQCP::~ResultPlotQCP()
{
    delete plot;
}

void ResultPlotQCP::refresh(void)
{
    int numPiles = m_pos.size();
    if (m_data.size() < numPiles) numPiles = m_data.size();
    if (numPiles < 1) return;

    //QVector<double> *xOffset;
    QVector<double> xOffset;

    int maxPts = -1;

    for (int i=0; i<numPiles; i++)
    {
        //if (m_pos[i] && m_pos[i]->size() > maxPts)
        //{
        //    maxPts = m_pos[i]->size();
        //    xOffset = m_pos[i];
        //}
        if (m_pos[i].size() > maxPts)
        {
            maxPts  = m_pos[i].size();
            xOffset = m_pos[i];
        }
    }

    plot->clearPlottables();

    plot->autoAddPlottableToLegend();
    plot->legend->setVisible(true);

    plot->addGraph();
    //plot->graph(0)->setData(QVector<double>(maxPts, 0.0),*xOffset);
    plot->graph(0)->setData(QVector<double>(maxPts, 0.0),xOffset);
    plot->graph(0)->setPen(QPen(Qt::black));
    plot->graph(0)->removeFromLegend();

    double xl =  99999.;
    double xr = -99999.;

    for (int ii=0; ii<numPiles; ii++) {
        QCPCurve *mCurve = new QCPCurve(plot->xAxis, plot->yAxis);
        //mCurve->setData(*m_data[ii],*m_pos[ii]);
        mCurve->setData(m_data[ii],m_pos[ii]);
        mCurve->setPen(QPen(LINE_COLOR[ii], 3));
        mCurve->setName(QString("Pile #%1").arg(ii+1));

        bool foundRange = false;
        QCPRange limits = mCurve->getKeyRange(foundRange);
        if (foundRange)
        {
            if (xl > limits.lower) xl = limits.lower;
            if (xr < limits.upper) xr = limits.upper;
        }
    }

    //
    // Plot Ground Layers
    //

    double range = (xr - xl) ;
    xl -= 0.1*range;
    xr += 0.1*range;

    for (int iLayer=0; iLayer<MAXLAYERS; iLayer++) {

        QVector<double> x(5,0.0);
        QVector<double> y(5,0.0);

        x[0] = xl;   y[0] = -depthOfLayer[iLayer];
        x[1] = xl;   y[1] = -depthOfLayer[iLayer+1];
        x[2] = xr;   y[2] = y[1];
        x[3] = xr;   y[3] = y[0];
        x[4] = x[0]; y[4] = y[0];

        QCPCurve *layerII = new QCPCurve(plot->xAxis, plot->yAxis);
        layerII->setData(x,y);
        layerII->setName(QString("Layer #%1").arg(iLayer+1));
        layerII->setPen(QPen(BRUSH_COLOR[iLayer], 1));
        layerII->setBrush(QBrush(BRUSH_COLOR[iLayer]));
    }

    //
    // setting up plot dimensions
    //
    //double L1 = (*m_pos[0]).last();
    double L1 = (m_pos[0]).last();

    //plot->setAxisScale( QwtPlot::yLeft, -depthOfLayer[3], L1 + 1.5);
    //plot->setAxisScale( QwtPlot::xBottom, xl, xr);

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    plot->axisRect()->autoMargins();
    plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignBottom);
    plot->rescaleAxes();
    plot->replot();
}

