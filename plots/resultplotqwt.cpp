/*!
 * \class ResultPlotQwt
 * \brief The ResultPlotQwt class implements the Qwt library interface for ResultPlotSuper.
 * \since 2.0
 * \ingroup Plotting
 *
 * This class provides library-dependent plotting for Qwt.
 *
 *  \sa ResultPlotQCP
 */


#include "resultplotqwt.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_legend.h"
#include <qwt_plot_shapeitem.h>
#include <qwt_plot_grid.h>

#include <QPolygonF>
#include <QPointF>
#include <QGridLayout>
#include <QPen>



ResultPlotQwt::ResultPlotQwt(QWidget *parent) :
    ResultPlotSuper(parent)
{
    plot = new QwtPlot(this);
    plot->setCanvasBackground(QBrush(Qt::white));
    grid = nullptr;

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
    int numPiles = m_pos.size();
    if (m_data.size() < numPiles) numPiles = m_data.size();
    if (numPiles < 1) return;

    if (grid)
    {
        delete grid;
        grid = nullptr;
    }

    //QVector<double> *xOffset;
    QVector<double> xOffset;

    int maxPts = -1;

    for (int i=0; i<numPiles; i++)
    {
        //if (m_pos[i]->size() > maxPts)
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
    //for (int i=0; i<maxPts; i++) { poly << QPointF( 0.0, (*xOffset)[i]); }
    for (int i=0; i<maxPts; i++) { poly << QPointF( 0.0, (xOffset)[i]); }
    curve->setSamples(poly);
    curve->setItemAttribute(QwtPlotItem::Legend, false);
    curve->attach(plot);


    double xl =  999.;
    double xr = -999.;

    //
    // plot results curves
    //

    for (int ii=0; ii<numPiles; ii++) {

        QwtPlotCurve *mCurve = new QwtPlotCurve();
        QPolygonF points;
        // for (int j=0; j<m_pos[ii]->length(); j++) {
        for (int j=0; j<m_pos[ii].length(); j++) {
            // points << QPointF( (*m_data[ii])[j],(*m_pos[ii])[j] );
            points << QPointF( (m_data[ii])[j],(m_pos[ii])[j] );
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
    if (range < 1e-5) range = 1e-5;
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
    //double L1 = (*m_pos[0]).last();
    double L1 = (m_pos[0]).last();

    plot->setAxisScale( QwtPlot::xBottom, xl, xr);
    plot->setAxisScale( QwtPlot::yLeft, -depthOfLayer[3], L1 + 1.75);

    // axes
    if (!m_posLabel.isEmpty()) {
        plot->setAxisTitle( QwtPlot::yLeft, m_posLabel );
    }
    if (!m_dataLabel.isEmpty()) {
        plot->setAxisTitle( QwtPlot::xBottom, m_dataLabel );
    }

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

