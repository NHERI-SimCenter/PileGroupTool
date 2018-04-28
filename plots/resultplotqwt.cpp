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

void ResultPlotQwt::plotResults(QVector<QVector<double> *> &y,
                                QVector<QVector<double> *> &x)
{
    /*
     * MAKE SURE THE LENGTH OF THE VECTORS IN ALL PLOTS MATCHES THE ACTUAL PILE
     *
     * NO MORE RANGE CHECKING!
     */
    if (grid)
    {
        delete grid;
        grid = NULL;
    }

    QVector<double> *xOffset;

    int numPiles = x.size();

    int maxPts = -1;

    for (int i=0; i<numPiles; i++)
    {
        if (x[i]->size() > maxPts)
        {
            maxPts = x[i]->size();
            xOffset = y[i];
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

    //plot->addGraph();
    //plot->graph(0)->setData(QVector<double>(maxPts, 0.0),*xOffset);
    //plot->graph(0)->setPen(QPen(Qt::black));
    //plot->graph(0)->removeFromLegend();

    QwtPlotCurve *curve = new QwtPlotCurve();
    curve->setPen(Qt::black, 1);
    QPolygonF poly;
    for (int i=0; i<maxPts; i++) { poly << QPointF( 0.0, (*xOffset)[i]); }
    curve->setSamples(poly);
    curve->setItemAttribute(QwtPlotItem::Legend, false);
    curve->attach(plot);

    //
    // Plot Ground Layers
    //
    /*
    for (int iLayer=0; iLayer<MAXLAYERS; iLayer++) {

        double xl = -50.;
        double xr =  50.;
        // Where is L1?
        double L1 = 1.0;
        double heightAbovePileCap = 1;
        double maxH = 0.5;
        //double maxD = 0.0;
        //double D = pileDiameter[pileIdx];
        //if (D>maxD) maxD = D;
        //maxH = maxD;
        //if (maxH > L1/2.) maxH = L1/2.;
        // Adjust y-axis to match Ground Layer Depth and slightly above pilecap
        plot->setAxisScale( QwtPlot::yLeft, -depthOfLayer[3], L1 + maxH + heightAbovePileCap );
        //qWarning() << QString::number(L1 + maxH + heightAbovePileCap);

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
    */


    // results curves
    for (int ii=0; ii<numPiles; ii++) {

        QwtPlotCurve *mCurve = new QwtPlotCurve();
        QPolygonF points;
        for (int j=0; j<x[ii]->length(); j++) {
            points << QPointF( (*x[ii])[j],(*y[ii])[j] );
        }

    mCurve->setSamples(points);
    mCurve->setPen(QPen(LINE_COLOR[ii], 3));
    mCurve->setTitle(QString("Pile #%1").arg(ii+1));
    mCurve->attach(plot);
    mCurve->setItemAttribute(QwtPlotItem::Legend, true);

    QRectF limits = mCurve->boundingRect();
    double range = limits.right()-limits.left();
    double xl = limits.left()  - 0.1*range;
    double xr = limits.right() + 0.1*range;

    plot->setAxisScale( QwtPlot::xBottom, xl, xr);

    for (int iLayer=0; iLayer<MAXLAYERS; iLayer++) {

        // These values should probably not be hard coded here
        // but they should be shared with the system plot
        double L1 = 1.0;
        double heightAbovePileCap = 1;
        double maxH = 0.5;

        plot->setAxisScale( QwtPlot::yLeft, -depthOfLayer[3],
                            L1 + maxH + heightAbovePileCap);

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

    }

    //plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    //plot->axisRect()->autoMargins();
    //plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignBottom);
    //plot->rescaleAxes();

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

