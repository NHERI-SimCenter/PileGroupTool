#include "resultplotqwt.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_legend.h"
#include <qwt_plot_shapeitem.h>
#include <qwt_plot_grid.h>

#include <QPolygonF>
#include <QPointF>
#include <QGridLayout>



ResultPlotQwt::ResultPlotQwt(QWidget *parent) :
    ResultPlotSuper(parent)
{
    plot = new QwtPlot(this);
    plot->setCanvasBackground(QBrush(Qt::white));
    grid = NULL;

    QGridLayout *lyt = new QGridLayout(this);
    lyt->addWidget(plot,0,0);
    lyt->setMargin(0);
    this->setLayout(lyt);

}

ResultPlotQwt::~ResultPlotQwt()
{
    delete plot;
}

void ResultPlotQwt::refresh(void)
{
    int numPiles = m_x.size();
    if (m_y.size() < numPiles) numPiles = m_y.size();
    if (numPiles < 1) return;

    if (grid)
    {
        delete grid;
        grid = NULL;
    }

    QVector<double> *xOffset;

    int maxPts = -1;

    for (int i=0; i<numPiles; i++)
    {
        if (m_x[i]->size() > maxPts)
        {
            maxPts = m_x[i]->size();
            xOffset = m_y[i];
        }
    }

    QwtPlotItemList list = plot->itemList();
    foreach (QwtPlotItem *item, list)
    {
        item->detach();
        delete item;
    }


    //plot->autoAddPlottableToLegend();

    //
    // Plot Legend
    //
    plot->insertLegend( new QwtLegend(), QwtPlot::BottomLegend );

    QwtPlotCurve *curve = new QwtPlotCurve();
    curve->setPen(Qt::black, 1);
    QPolygonF poly;
    for (int i=0; i<maxPts; i++) { poly << QPointF( 0.0, (*xOffset)[i]); }
    curve->setSamples(poly);
    curve->setItemAttribute(QwtPlotItem::Legend, false);
    curve->attach(plot);


    double xl =  99999.;
    double xr = -99999.;

    //
    // plot results curves
    //

    for (int ii=0; ii<numPiles; ii++) {

        QwtPlotCurve *mCurve = new QwtPlotCurve();
        QPolygonF points;
        for (int j=0; j<m_x[ii]->length(); j++) {
            points << QPointF( (*m_x[ii])[j],(*m_y[ii])[j] );
        }

        mCurve->setSamples(points);
        mCurve->setPen(QPen(LINE_COLOR[ii], 3));
        mCurve->setTitle(QString("Pile #%1").arg(ii+1));
        mCurve->attach(plot);
        mCurve->setItemAttribute(QwtPlotItem::Legend, true);

        QRectF limits = mCurve->boundingRect();
        if (xl > limits.left())  xl = limits.left();
        if (xr < limits.right()) xr = limits.right();
    }

    //
    // Plot Ground Layers
    //

    double range = (xr - xl) ;
    xl -= 0.1*range;
    xr += 0.1*range;

    for (int iLayer=0; iLayer<MAXLAYERS; iLayer++) {

        QPolygonF groundCorners;
        groundCorners << QPointF(xl , -depthOfLayer[iLayer]  )
                      << QPointF(xl , -depthOfLayer[iLayer+1])
                << QPointF(xr , -depthOfLayer[iLayer+1])
                << QPointF(xr , -depthOfLayer[iLayer]  )
                << QPointF(xl , -depthOfLayer[iLayer]  );

        QwtPlotShapeItem *layerII = new QwtPlotShapeItem();

        layerII->setPolygon(groundCorners);
        layerII->setPen(QPen(BRUSH_COLOR[iLayer], 1));
        layerII->setBrush(QBrush(BRUSH_COLOR[iLayer]));
        layerII->setZ(0);
        layerII->attach( plot );
        layerII->setItemAttribute(QwtPlotItem::Legend, false);
    }

    //
    // setting up plot dimensions
    //
    double L1 = (*m_x[0]).last();

    plot->setAxisScale( QwtPlot::xBottom, xl, xr);
    plot->setAxisScale( QwtPlot::yLeft, -depthOfLayer[3], L1 + 1.50);

    // Create Background Grid for Plot
    grid = new QwtPlotGrid();
    grid->setMajorPen(QPen(Qt::darkGray, 0.8));
    grid->setMinorPen(QPen(Qt::lightGray, 0.5));
    grid->setZ(1);
    grid->enableX(true);
    grid->enableY(true);
    grid->enableXMin(true);
    grid->enableYMin(true);

    grid->attach( plot );

    plot->replot();
}

