#include "systemplotqwt.h"
#include "systemplotsuper.h"

#include <qwt_plot.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_plot_shapeitem.h>

#include <QDebug>
#include <QTime>


SystemPlotQwt::SystemPlotQwt(QWidget *parent) :
    SystemPlotSuper(parent)
{
    //
    // create a QwtPlot
    //
    plot = new QwtPlot(this);
    plotItemList.clear();

    // Create Background Grid for Plot
    grid = new QwtPlotGrid();
    grid->setMajorPen(QPen(Qt::lightGray, 0.8));
    grid->attach( plot );

    // Layout plot
    QGridLayout *lyt = new QGridLayout(this);
    lyt->addWidget(plot,0,0);
    lyt->setMargin(0);
    this->setLayout(lyt);


#if 0
    //
    // add legend
    //
    // now we move the legend from the inset layout of the axis rect into the main grid layout.
    // We create a sub layout so we can generate a small gap between the plot layout cell border
    // and the legend border:
    QCPLayoutGrid *subLayout = new QCPLayoutGrid;
    plot->plotLayout()->addElement(1, 0, subLayout);
    subLayout->setMargins(QMargins(5, 0, 5, 5));
    subLayout->addElement(0, 0, plot->legend);
    //
    // change the fill order of the legend, so it's filled left to right in columns:
    plot->legend->setWrap(4);
    plot->legend->setRowSpacing(1);
    plot->legend->setColumnSpacing(2);

    // set legend's row stretch factor very small so it ends up with minimum height:
    plot->plotLayout()->setRowStretchFactor(1, 0.001);

    QObject::connect(plot, SIGNAL(selectionChangedByUser()), this, SLOT(on_plot_selectionChangedByUser()));
#endif

    //
    // default plot selection settings
    //
    activePileIdx = 0;
    activeLayerIdx = -1;
}

SystemPlotQwt::~SystemPlotQwt()
{
    delete plot;
}

void SystemPlotQwt::refresh()
{  
    //qDebug() << "entering SystemPlotQwt::refresh()" << QTime::currentTime();
    foreach (QwtPlotItem *item, plotItemList) {
        item->detach();
        delete item;
    }
    plotItemList.clear();

    for (int k=0; k<MAXPILES; k++) {
        headNodeList[k] = {-1, -1, 0.0, 1.0, 1.0};
    }

    //
    // find dimensions for plotting
    //
    //QVector<double> depthOfLayer = QVector<double>(4, 0.0); // add a buffer element for bottom of the third layer

    /* ******** sizing and adjustments ******** */

    double minX0 =  999999.;
    double maxX0 = -999999.;
    double xbar  = 0.0;
    double H     = 0.0;
    double W, WP;
    double depthSoil = 0.0;
    double nPiles = 0.;
    double maxD = 0.0;
    double maxH = 0.0;

    //
    // find depth of defined soil layers
    //
    depthSoil = depthOfLayer[3];

    for (int pileIdx=0; pileIdx<numPiles; pileIdx++) {
        if ( xOffset[pileIdx] < minX0) { minX0 = xOffset[pileIdx]; }
        if ( xOffset[pileIdx] > maxX0) { maxX0 = xOffset[pileIdx]; }
        if (L2[pileIdx] > H) { H = L2[pileIdx]; }
        double D = pileDiameter[pileIdx];
        if (D>maxD) maxD = D;

        xbar += xOffset[pileIdx]; nPiles++;
    }
    xbar /= nPiles;
    if (depthSoil > H) H = depthSoil;
    if ( L1 > 0.0 ) H += L1;

    WP = maxX0 - minX0;
    W  = (1.10*WP + 0.50*H);
    if ( (WP + 0.35*H) > W ) { W = WP + 0.35*H; }

    maxH = maxD;
    if (maxH > L1/2.) maxH = L1/2.;


#if 0
    // setup system plot
    plot->clearPlottables();
    plot->clearGraphs();
    plot->clearItems();

    if (!plot->layer("groundwater"))
        { plot->addLayer("groundwater", plot->layer("grid"), QCustomPlot::limAbove); }
    if (!plot->layer("soil"))
        { plot->addLayer("soil", plot->layer("groundwater"), QCustomPlot::limAbove); }
    if (!plot->layer("piles"))
        { plot->addLayer("piles", plot->layer("soil"), QCustomPlot::limAbove); }
#endif

#if 0
    plot->autoAddPlottableToLegend();
    plot->legend->setVisible(true);
#endif

#if 0

    QVector<double> zero(2,xbar-0.5*W);
    QVector<double> loc(2,0.0);
    loc[0] = -(H-L1); loc[1] = L1;

    plot->addGraph();
    plot->graph(0)->setData(zero,loc);
    plot->graph(0)->setPen(QPen(Qt::black,1));
    plot->graph(0)->removeFromLegend();

    // create layers

    plot->setCurrentLayer("soil");

#endif

    //
    // HERE IS WHERE TO START ...
    //

    // Legend
    plot->insertLegend( new QwtLegend(), QwtPlot::BottomLegend );


    /* Temp data to check if legend is working
    QwtPlotCurve *curve = new QwtPlotCurve();
    curve->setTitle( "Random Points" );
    curve->setPen( LINE_COLOR[3], 2 );
    curve->setBrush( GROUND_WATER_BLUE );

    QPolygonF points;
    points << QPointF( 2, 5 ) << QPointF( 2, 7 )
        << QPointF( 4, 7 ) << QPointF( 4, 5 )
        << QPointF( 2, 5 );

    curve->setSamples( points);
    curve->attach( plot );
    plotItemList.append(curve);
    // End Temp data*/

    // Ground Layers
    for (int iLayer=0; iLayer<MAXLAYERS; iLayer++) {

        QVector<double> x(5,0.0);
        QVector<double> y(5,0.0);

        x[0] = xbar - W/2.; y[0] = -depthOfLayer[iLayer];
        x[1] = x[0];        y[1] = -depthOfLayer[iLayer+1];
        x[2] = xbar + W/2.; y[2] = y[1];
        x[3] = x[2];        y[3] = y[0];
        x[4] = x[0];        y[4] = y[0];

        QwtPlotCurve *layerII = new QwtPlotCurve();
        layerII->setSamples(x,y);
        layerII->setTitle(QString("Layer #%1").arg(iLayer+1));

        if (iLayer == activeLayerIdx) {
            layerII->setPen(QPen(Qt::red, 2));
            layerII->setBrush(QBrush(BRUSH_COLOR[3+iLayer]));
        }
        else {
            layerII->setPen(QPen(BRUSH_COLOR[iLayer], 1));
            layerII->setBrush(QBrush(BRUSH_COLOR[iLayer]));
        }

        layerII->attach( plot );
        plotItemList.append(layerII);
    }


    // Adjust x-axis to match Ground Layer Width
    plot->setAxisScale( QwtPlot::xBottom, xbar - W/2, xbar + W/2 );




#if 0
    for (int iLayer=0; iLayer<MAXLAYERS; iLayer++) {

        QVector<double> x(5,0.0);
        QVector<double> y(5,0.0);

        x[0] = xbar - W/2.; y[0] = -depthOfLayer[iLayer];
        x[1] = x[0];        y[1] = -depthOfLayer[iLayer+1];
        x[2] = xbar + W/2.; y[2] = y[1];
        x[3] = x[2];        y[3] = y[0];
        x[4] = x[0];        y[4] = y[0];

        QCPCurve *layerII = new QCPCurve(plot->xAxis, plot->yAxis);
        layerII->setData(x,y);
        layerII->setName(QString("Layer #%1").arg(iLayer+1));

        if (iLayer == activeLayerIdx) {
            layerII->setPen(QPen(Qt::red, 2));
            layerII->setBrush(QBrush(BRUSH_COLOR[3+iLayer]));
        }
        else {
            layerII->setPen(QPen(BRUSH_COLOR[iLayer], 1));
            layerII->setBrush(QBrush(BRUSH_COLOR[iLayer]));
        }
    }

#endif


    // ground water table

    // plot->setCurrentLayer("groundwater");

    if (gwtDepth < (H-L1)) {
        QVector<double> x(5,0.0);
        QVector<double> y(5,0.0);

        x[0] = xbar - W/2.; y[0] = -gwtDepth;
        x[1] = x[0];        y[1] = -(H - L1);
        x[2] = xbar + W/2.; y[2] = y[1];
        x[3] = x[2];        y[3] = y[0];
        x[4] = x[0];        y[4] = y[0];

        QwtPlotCurve *water = new QwtPlotCurve();
        water->setSamples(x,y);

        water->setPen(QPen(Qt::blue, 2));
        water->setBrush(QBrush(GROUND_WATER_BLUE));

        water->setTitle(QString("Groundwater"));
        water->attach( plot);
        plotItemList.append(water);
    }



#if 0
    // ground water table

    plot->setCurrentLayer("groundwater");

    if (gwtDepth < (H-L1)) {
        QVector<double> x(5,0.0);
        QVector<double> y(5,0.0);

        x[0] = xbar - W/2.; y[0] = -gwtDepth;
        x[1] = x[0];        y[1] = -(H - L1);
        x[2] = xbar + W/2.; y[2] = y[1];
        x[3] = x[2];        y[3] = y[0];
        x[4] = x[0];        y[4] = y[0];

        QCPCurve *water = new QCPCurve(plot->xAxis, plot->yAxis);
        water->setData(x,y);

        water->setPen(QPen(Qt::blue, 2));
        water->setBrush(QBrush(GROUND_WATER_BLUE));

        water->setName(QString("groundwater"));
    }
#endif

    // plot the pile cap

    //plot->setCurrentLayer("piles");

    QVector<double> x(5,0.0);
    QVector<double> y(5,0.0);

    x[0] = minX0 - maxD/2.; y[0] = L1 + maxH;
    x[1] = x[0];            y[1] = L1 - maxH;
    x[2] = maxX0 + maxD/2.; y[2] = y[1];
    x[3] = x[2];            y[3] = y[0];
    x[4] = x[0];            y[4] = y[0];

    QwtPlotCurve *pileCap = new QwtPlotCurve();
    pileCap->setSamples(x,y);
    pileCap->setPen(QPen(Qt::black, 1));
    pileCap->setBrush(QBrush(Qt::gray));
    pileCap->setTitle(QString("Pile Cap"));
    pileCap->attach( plot );
    //pileCap->setZ(3);
    plotItemList.append(pileCap);

    pileCap->setItemAttribute(QwtPlotItem::Legend, false);



    // plot the piles
    for (int pileIdx=0; pileIdx<numPiles; pileIdx++) {

        QVector<double> x(5,0.0);
        QVector<double> y(5,0.0);

        double D = pileDiameter[pileIdx];

        x[0] = xOffset[pileIdx] - D/2.; y[0] = L1;
        x[1] = x[0];                    y[1] = -L2[pileIdx];
        x[2] = xOffset[pileIdx] + D/2.; y[2] = y[1];
        x[3] = x[2];                    y[3] = y[0];
        x[4] = x[0];                    y[4] = y[0];

        QwtPlotCurve *pileII = new QwtPlotCurve();
        pileII->setSamples(x,y);
        if (pileIdx == activePileIdx) {
            pileII->setPen(QPen(Qt::red, 2));
            pileII->setBrush(QBrush(BRUSH_COLOR[9+pileIdx]));
        }
        else {
            pileII->setPen(QPen(Qt::black, 1));
            pileII->setBrush(QBrush(BRUSH_COLOR[6+pileIdx]));
        }
        pileII->setTitle(QString("Pile #%1").arg(pileIdx+1));
        pileII->attach( plot);
        //pileII -> setZ(2);
        plotItemList.append(pileII);
    }

#if 0
    // plot the pile cap

    plot->setCurrentLayer("piles");

    QVector<double> x(5,0.0);
    QVector<double> y(5,0.0);

    x[0] = minX0 - maxD/2.; y[0] = L1 + maxH;
    x[1] = x[0];            y[1] = L1 - maxH;
    x[2] = maxX0 + maxD/2.; y[2] = y[1];
    x[3] = x[2];            y[3] = y[0];
    x[4] = x[0];            y[4] = y[0];

    QCPCurve *pileCap = new QCPCurve(plot->xAxis, plot->yAxis);
    pileCap->setData(x,y);
    pileCap->setPen(QPen(Qt::black, 1));
    pileCap->setBrush(QBrush(Qt::gray));
    pileCap->removeFromLegend();

    // plot the piles
    for (int pileIdx=0; pileIdx<numPiles; pileIdx++) {

        QVector<double> x(5,0.0);
        QVector<double> y(5,0.0);

        double D = pileDiameter[pileIdx];

        x[0] = xOffset[pileIdx] - D/2.; y[0] = L1;
        x[1] = x[0];                    y[1] = -L2[pileIdx];
        x[2] = xOffset[pileIdx] + D/2.; y[2] = y[1];
        x[3] = x[2];                    y[3] = y[0];
        x[4] = x[0];                    y[4] = y[0];

        QCPCurve *pileII = new QCPCurve(plot->xAxis, plot->yAxis);
        pileII->setData(x,y);
        if (pileIdx == activePileIdx) {
            pileII->setPen(QPen(Qt::red, 2));
            pileII->setBrush(QBrush(BRUSH_COLOR[9+pileIdx]));
        }
        else {
            pileII->setPen(QPen(Qt::black, 1));
            pileII->setBrush(QBrush(BRUSH_COLOR[6+pileIdx]));
        }
        pileII->setName(QString("Pile #%1").arg(pileIdx+1));
    }
#endif


    // Drawing Horizontal Force Arrow using QwtPlotShapeItem
    //
    QwtPlotShapeItem *arrow = new QwtPlotShapeItem();

    double forceArrowRatio = -P/MAX_FORCE;

    // Defining minimum size of the horizontal force arrow
    double forceArrowMin = 0.03;
    if (( forceArrowRatio < forceArrowMin ) && (forceArrowRatio > 0)) {
        forceArrowRatio = forceArrowMin;
    }
    else if (( forceArrowRatio > -forceArrowMin ) && (forceArrowRatio < 0)) {
        forceArrowRatio = -forceArrowMin;
    }

    QPen pen( Qt::black, 2 );
    pen.setJoinStyle( Qt::MiterJoin );
    arrow->setPen( pen );
    arrow->setBrush( Qt::red );

    double pileCapCenter = 0.5 * (minX0 + maxX0);
    double arrowThickness = 0.1, arrowHead = 0.4, arrowHeadLength = 0.5;

    if (forceArrowRatio < 0) {arrowHeadLength = -arrowHeadLength;}

    QPainterPath path;
    path.moveTo( pileCapCenter, L1 + maxH );
    path.lineTo( pileCapCenter + arrowHeadLength, L1 + maxH + arrowHead );
    path.lineTo( pileCapCenter + arrowHeadLength, L1 + maxH + arrowThickness );
    path.lineTo( pileCapCenter + arrowHeadLength + forceArrowRatio*( W/2 ), L1 + maxH + arrowThickness );
    path.lineTo( pileCapCenter + arrowHeadLength + forceArrowRatio*( W/2 ), L1 + maxH - arrowThickness );
    path.lineTo( pileCapCenter + arrowHeadLength, L1 + maxH - arrowThickness );
    path.lineTo( pileCapCenter + arrowHeadLength, L1 + maxH - arrowHead );
    path.lineTo( pileCapCenter, L1 + maxH );
    arrow->setShape( path );
    //arrow->setZ	( 4 );

    if (forceArrowRatio != 0){
    arrow->attach( plot );
    plotItemList.append(arrow);
    }

    //
    // QwtPlotShapeItem version end


    // Drawing Vertical Force Arrow using QwtPlotShapeItem

    /*
    QwtPlotShapeItem *arrowV = new QwtPlotShapeItem();

    double forceArrowRatioV = PV/MAX_FORCE;

    if (( forceArrowRatioV < 0.3 ) && (forceArrowRatioV > 0)) {
        forceArrowRatio = 0.3;
    }
    else if (( forceArrowRatioV > -0.3 ) && (forceArrowRatioV < 0)) {
        forceArrowRatioV = -0.3;
    }

    //QPen pen( Qt::black, 2 );
    //pen.setJoinStyle( Qt::MiterJoin );
    arrowV->setPen( pen );
    arrowV->setBrush( Qt::red );

    QPainterPath pathV;
    pathV.moveTo( pileCapCenter, L1 + maxH );
    pathV.lineTo( pileCapCenter - 1, L1 + maxH + 1 );
    pathV.lineTo( pileCapCenter - 0.25, L1 + maxH + 1 );
    pathV.lineTo( pileCapCenter - 0.25, L1 + maxH + 5 );
    pathV.lineTo( pileCapCenter + 0.25, L1 + maxH + 5 );
    pathV.lineTo( pileCapCenter + 0.25, L1 + maxH + 1 );
    pathV.lineTo( pileCapCenter + 1, L1 + maxH + 1 );
    pathV.lineTo( pileCapCenter, L1 + maxH );
    arrowV->setShape( path );
    arrowV->setZ	( 3 );

    if (forceArrowRatioV != 0){
    arrowV->attach( plot );
    plotItemList.append(arrow);
    }
    */

#if 0
    // add force to the plot

    if (ABS(P) > 0.0) {
        double force = 0.45*W*P/MAX_FORCE;

        // add the arrow:
        QCPItemLine *arrow = new QCPItemLine(plot);
        //plot->addItem(arrow);
        arrow->setPen(QPen(Qt::red, 3));
        arrow->start->setCoords(xbar, L1);
        arrow->end->setCoords(xbar + force, L1);
        arrow->setHead(QCPLineEnding::esSpikeArrow);
    }

    if (ABS(PV) > 0.0) {
        double force = 0.45*W*PV/MAX_FORCE;

        // add the arrow:
        QCPItemLine *arrow = new QCPItemLine(plot);
        //plot->addItem(arrow);
        arrow->setPen(QPen(Qt::red, 3));
        arrow->start->setCoords(xbar, L1 - force);
        arrow->end->setCoords(xbar, L1);
        arrow->setHead(QCPLineEnding::esSpikeArrow);
    }

    // plot scaling options

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    plot->xAxis->setScaleRatio(plot->yAxis);
    plot->rescaleAxes();
#endif
    plot->replot();

}
