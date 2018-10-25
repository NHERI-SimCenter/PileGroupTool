#include "systemplotqwt.h"
#include "systemplotsuper.h"

#include <qwt_plot.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_plot_shapeitem.h>
#include <qwt_plot_textlabel.h>
#include <qwt_text.h>

#include "qwt_picker.h"
#include "qwt_plot_picker.h"
#include "qwt_plot_item.h"
#include "qwt_plot_shapeitem.h"
#include "qwt_picker_machine.h"
#include "qwt_scale_map.h"

#include <QDebug>
#include <QTime>
#include <QFont>


SystemPlotQwt::SystemPlotQwt(QWidget *parent) :
    SystemPlotSuper(parent)
{
    //
    // Create a QwtPlot
    //
    plot = new QwtPlot(this);
    plot->setCanvasBackground(QBrush(Qt::white));
    plotItemList.clear();

    // Create Background Grid for Plot
    grid = new QwtPlotGrid();
    grid->setMajorPen(QPen(Qt::lightGray, 0.8));
    grid->setZ(1);
    grid->attach( plot );

    // Layout Plot
    QGridLayout *lyt = new QGridLayout(this);
    lyt->addWidget(plot,0,0);
    lyt->setMargin(0);
    this->setLayout(lyt);

    //Picker
    QwtPicker *picker = new QwtPicker(plot->canvas());
    picker->setStateMachine(new QwtPickerClickPointMachine);
    picker->setTrackerMode(QwtPicker::AlwaysOff);
    picker->setRubberBand(QwtPicker::RectRubberBand);

    //connect(picker, SIGNAL(activated(bool)), this, SLOT(on_picker_activated(bool)));
    //connect(picker, SIGNAL(selected(const QPolygon &)), this, SLOT(on_picker_selected(const QPolygon &)));
    connect(picker, SIGNAL(appended(const QPoint &)), this, SLOT(on_picker_appended(const QPoint &)));
    //connect(picker, SIGNAL(moved(const QPoint &)), this, SLOT(on_picker_moved(const QPoint &)));
    //connect(picker, SIGNAL(removed(const QPoint &)), this, SLOT(on_picker_removed(const QPoint &)));
    //connect(picker, SIGNAL(changed(const QPolygon &)), this, SLOT(on_picker_changed(const QPolygon &)));

    //
    // default plot selection settings
    //
    activePileIdx  = 0;
    activeLayerIdx = -1;
}

SystemPlotQwt::~SystemPlotQwt()
{
    delete plot;
}


PLOTOBJECT SystemPlotQwt::itemAt( const QPoint& pos ) const
{
    PLOTOBJECT emptyObj;
    emptyObj.itemPtr = nullptr;
    emptyObj.type    = PLType::NONE;
    emptyObj.index   = -1;

    if ( plot == nullptr )
        return emptyObj;

    // translate pos into the plot coordinates
    double coords[ QwtPlot::axisCnt ];
    coords[ QwtPlot::xBottom ] = plot->canvasMap( QwtPlot::xBottom ).invTransform( pos.x() );
    coords[ QwtPlot::xTop ]    = plot->canvasMap( QwtPlot::xTop ).invTransform( pos.x() );
    coords[ QwtPlot::yLeft ]   = plot->canvasMap( QwtPlot::yLeft ).invTransform( pos.y() );
    coords[ QwtPlot::yRight ]  = plot->canvasMap( QwtPlot::yRight ).invTransform( pos.y() );

    //qDebug() << "Pos: " << pos
    //         << " coord: " << coords[ QwtPlot::xBottom ]
    //         << "/" << coords[ QwtPlot::xTop ]
    //         << "/" << coords[ QwtPlot::yLeft ]
    //         << "/" << coords[ QwtPlot::yRight ];

    for ( int i = plotItemList.size() - 1; i >= 0; i-- )
    {
        PLOTOBJECT obj = plotItemList[i];
        QwtPlotItem *item = obj.itemPtr;
        if ( item->isVisible() && item->rtti() == QwtPlotItem::Rtti_PlotCurve )
        {
            double dist;

            QwtPlotCurve *curveItem = static_cast<QwtPlotCurve *>( item );
            const QPointF p( coords[ item->xAxis() ], coords[ item->yAxis() ] );

            if ( curveItem->boundingRect().contains( p ) )
            {
                // trace curves ...
                dist = 1000.;
                for (size_t line=0; line < curveItem->dataSize() - 1; line++)
                {
                    QPointF pnt;
                    double x, y;

                    pnt = curveItem->sample(line);
                    x = plot->canvasMap( QwtPlot::xBottom ).transform( pnt.x() );
                    y = plot->canvasMap( QwtPlot::yLeft ).transform( pnt.y() );
                    QPointF x0(x,y);

                    pnt = curveItem->sample(line+1);
                    x = plot->canvasMap( QwtPlot::xBottom ).transform( pnt.x() );
                    y = plot->canvasMap( QwtPlot::yLeft ).transform( pnt.y() );
                    QPointF x1(x,y);

                    QPointF r  = pos - x0;
                    QPointF s  = x1 - x0;
                    double s2  = QPointF::dotProduct(s,s);

                    if (s2 > 1e-6)
                    {
                        double xi  = QPointF::dotProduct(r,s) / s2;

                        if ( 0.0 <= xi && xi <= 1.0 )
                        {
                            QPointF t(-s.y()/sqrt(s2), s.x()/sqrt(s2));
                            double d1 = QPointF::dotProduct(r,t);
                            if ( d1 < 0.0 )  { d1 = -d1; }
                            if ( d1 < dist ) { dist = d1;}
                        }
                    }
                    else
                    {
                        dist = sqrt(QPointF::dotProduct(r,r));
                        //QPointF r2 = pos - x1;
                        double d2  = sqrt(QPointF::dotProduct(r,r));
                        if ( d2 < dist ) { dist = d2; }
                    }
                }

                qWarning() << "curve dist =" << dist;

                if ( dist <= 5 ) return obj;
            }
        }
        if ( item->isVisible() && item->rtti() == QwtPlotItem::Rtti_PlotShape )
        {
            QwtPlotShapeItem *shapeItem = static_cast<QwtPlotShapeItem *>( item );
            const QPointF p( coords[ item->xAxis() ], coords[ item->yAxis() ] );

            if ( shapeItem->boundingRect().contains( p ) && shapeItem->shape().contains( p ) )
            {
                return obj;
            }
        }
    }

    return emptyObj;
}

void SystemPlotQwt::on_picker_appended (const QPoint &pos)
{
    //SystemPlotQwt::refresh();

    PLOTOBJECT    obj = itemAt(pos);
    QwtPlotItem *item = obj.itemPtr;

    if ( item )
    {
        plot->replot();

        switch (obj.type) {
        case PLType::PILE:
            setActiveLayer(-1);
            setActivePile(obj.index);
            emit on_pileSelected(obj.index);
            break;
        case PLType::SOIL:
            setActivePile(-1);
            setActiveLayer(obj.index);
            emit on_soilLayerSelected(obj.index);
            break;
        case PLType::WATER:
            emit on_groundWaterSelected();
            break;
        case PLType::CAP:
        case PLType::LOAD:
        case PLType::OTHER:
        case PLType::NONE:
            break;
        }
    }
}


void SystemPlotQwt::refresh()
{  
    foreach (PLOTOBJECT item, plotItemList) {
        item.itemPtr->detach();
        delete item.itemPtr;
        //item.itemPtr = nullptr;
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


    double xl = xbar - W/2;
    double xr = xbar + W/2;

    // Plot Legend
    plot->insertLegend( new QwtLegend(), QwtPlot::BottomLegend );

    // Set x-axis and y-axis
    if (loadControlType == LoadControlType::SoilMotion)
    {
        plot->setAxisScale( QwtPlot::xBottom, xl-1.0, xr+1.0 );
        plot->setAxisScale( QwtPlot::xTop, xl-1.0, xr+1.0 );
    }
    else
    {
        plot->setAxisScale( QwtPlot::xBottom, xl, xr );
        plot->setAxisScale( QwtPlot::xTop, xl, xr );
    }
    plot->setAxisScale( QwtPlot::yLeft, -depthOfLayer[3], L1 + 1.75);
    plot->setAxisScale( QwtPlot::yRight, -depthOfLayer[3], L1 + 1.75);

    //
    // Plot Ground Water Table
    //

    if (gwtDepth < (H-L1)) {
        QPolygonF(groundwaterCorners);
        double s = soilMotion.last();
        s = shift(gwtDepth);

        groundwaterCorners << QPointF(xl + s, -gwtDepth);

        for (int i=0; i<=MAXLAYERS; i++)
        {
            if (gwtDepth <= depthOfLayer[i])
            {
                s = shift(depthOfLayer[i]);
                groundwaterCorners << QPointF(xl + s, -depthOfLayer[i]);
            }
        }

        s = shift((H - L1));
        groundwaterCorners << QPointF(xl + s, -(H - L1))
                           << QPointF(xr + s, -(H - L1));

        for (int i=MAXLAYERS; i>=0; i--)
        {
            if (gwtDepth <= depthOfLayer[i])
            {
                s = shift(depthOfLayer[i]);
                groundwaterCorners << QPointF(xr + s, -depthOfLayer[i]);
            }
        }

        s = shift(gwtDepth);
        groundwaterCorners << QPointF(xr + s, -gwtDepth)
                           << QPointF(xl + s, -gwtDepth);

        QwtPlotShapeItem *water = new QwtPlotShapeItem();
        water->setPolygon(groundwaterCorners);

        water->setPen(QPen(Qt::blue, 2));
        water->setBrush(QBrush(GROUND_WATER_BLUE));

        water->setTitle(QString("Groundwater"));
        water->attach( plot );
        water->setItemAttribute(QwtPlotItem::Legend, true);

        PLOTOBJECT var;
        var.itemPtr = water;
        var.type    = PLType::WATER;
        var.index   = -1;
        plotItemList.append(var);
    }

    //
    // Plot Ground Layers
    //
    for (int iLayer=0; iLayer<MAXLAYERS; iLayer++) {

        QPolygonF groundCorners;
        groundCorners << QPointF(xl + shift(depthOfLayer[iLayer])  , -depthOfLayer[iLayer]  )
                      << QPointF(xl + shift(depthOfLayer[iLayer+1]), -depthOfLayer[iLayer+1])
                      << QPointF(xr + shift(depthOfLayer[iLayer+1]), -depthOfLayer[iLayer+1])
                      << QPointF(xr + shift(depthOfLayer[iLayer])  , -depthOfLayer[iLayer]  )
                      << QPointF(xl + shift(depthOfLayer[iLayer])  , -depthOfLayer[iLayer]  );

        QwtPlotShapeItem *layerII = new QwtPlotShapeItem();

        layerII->setPolygon(groundCorners);
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
        layerII->setItemAttribute(QwtPlotItem::Legend, true);

        PLOTOBJECT var;
        var.itemPtr = layerII;
        var.type    = PLType::SOIL;
        var.index   = iLayer;
        plotItemList.append(var);
    }

    //
    // Plot the Pile Cap
    //
    QRectF capCorners(QPointF(minX0 - maxD/2, L1 + maxH), QSizeF(maxX0 + maxD - minX0, -maxH));

    QwtPlotShapeItem *pileCap = new QwtPlotShapeItem();
    pileCap->setRect(capCorners);
    pileCap->setPen(QPen(Qt::black, 1));
    pileCap->setBrush(QBrush(Qt::gray));
    pileCap->attach( plot );

    PLOTOBJECT var;
    var.itemPtr = pileCap;
    var.type    = PLType::CAP;
    var.index   = -1;
    plotItemList.append(var);

    pileCap->setItemAttribute(QwtPlotItem::Legend, false);

    //
    // Plot Piles
    //

    for (int pileIdx=0; pileIdx<numPiles; pileIdx++) {

        double D = pileDiameter[pileIdx];
        QPolygonF(pileCorners);

        if (m_pos.size() == numPiles)
        {
            // deformed pile

            for (int i=0; i<m_pos[pileIdx]->size(); i++)
            {
                pileCorners << QPointF( xOffset[pileIdx]     + (*m_dispU[pileIdx])[i] + D/2.,
                                        (*m_pos[pileIdx])[i] + (*m_dispV[pileIdx])[i] );
            }

            for (int i=m_pos[pileIdx]->size()-1; i>=0; i--)
            {
                pileCorners << QPointF( xOffset[pileIdx]     + (*m_dispU[pileIdx])[i] - D/2.,
                                        (*m_pos[pileIdx])[i] + (*m_dispV[pileIdx])[i] );
            }
        }
        else
        {
            // undeformed pile
            pileCorners << QPointF(xOffset[pileIdx] - D/2, L1)
                        << QPointF(xOffset[pileIdx] - D/2, -L2[pileIdx])
                        << QPointF(xOffset[pileIdx] + D/2, -L2[pileIdx])
                        << QPointF(xOffset[pileIdx] + D/2, L1)
                        << QPointF(xOffset[pileIdx] - D/2, L1);
        }

        QwtPlotShapeItem *pileII = new QwtPlotShapeItem();
        pileII->setPolygon(pileCorners);
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
        pileII->setItemAttribute(QwtPlotItem::Legend, true);

        PLOTOBJECT var;
        var.itemPtr = pileII;
        var.type    = PLType::PILE;
        var.index   = pileIdx;
        plotItemList.append(var);
    }

    double pileCapCenter = 0.5 * (minX0 + maxX0);
    QPointF forceOrigin  = QPointF(pileCapCenter, L1 + maxH);

    //
    // Plot Scaling Parameters
    //

    QPointF plotZero     = QPointF(0.0, 0.0);
    QPointF deltaPoint   = QPointF(5.0, 5.0);

    double deltaPointx   = plot->canvasMap( QwtPlot::xBottom).transform(deltaPoint.x());
    double zeroPointx    = plot->canvasMap( QwtPlot::xBottom).transform(plotZero.x());
    double deltaPointy   = plot->canvasMap( QwtPlot::yLeft).transform(deltaPoint.y());
    double zeroPointy    = plot->canvasMap( QwtPlot::yLeft).transform(plotZero.y());

    double xScalar = 5.0/(deltaPointx-zeroPointx);
    double yScalar = 5.0/(deltaPointy-zeroPointy);

    //qDebug()   << "zeroPoint  (x,y) = (" << zeroPointx << "," << zeroPointy << ")\n"
    //           << "deltaPoint (x,y) = (" << deltaPointx << "," << deltaPointy << ")\n"
    //           << "Scalar     (x,y) = (" << xScalar << "," << yScalar << ")\n";

    //
    // Plotting the Applied Moment Symbol
    //
    double alpha;
    double dTheta;
    double theta;

    double mR = 25.0; // Moment Radius in screen coordinates
    double nChords = 20;

    double Rx = mR*xScalar;
    double Ry = mR*yScalar;

    QwtPlotCurve *moment = new QwtPlotCurve();
    QPolygonF pointsOnMomentCurve;

    if (PMom < 0)
    {
        alpha = (1.0 + 0.75*(PMom)/MAX_MOMENT) * (2*3.1415927);
        dTheta = (alpha - 2*3.1415927) / (nChords-1);
        theta = 0.5*(3.1415927 - alpha);
    }
    else
    {
        alpha = (1.0 - 0.75*(PMom)/MAX_MOMENT) * (2*3.1415927);
        dTheta = (2*3.1415927 - alpha) / (nChords-1);
        theta = 0.5*(alpha + 3.1415927);
    }

    for (int n=0; n<=nChords; n++)
    {
        pointsOnMomentCurve << (forceOrigin + QPointF(Rx*cos(theta), Ry*sin(theta)));
        theta += dTheta;
    }
    theta -= dTheta;
    pointsOnMomentCurve << (forceOrigin + QPointF(0.8*Rx*cos(theta-3*dTheta), 0.8*Ry*sin(theta-3*dTheta)));
    pointsOnMomentCurve << (forceOrigin + QPointF(1.2*Rx*cos(theta-3*dTheta), 1.2*Ry*sin(theta-3*dTheta)));
    pointsOnMomentCurve << (forceOrigin + QPointF(Rx*cos(theta), Ry*sin(theta)));

    moment->setSamples(pointsOnMomentCurve);
    moment->setItemAttribute(QwtPlotItem::Legend, false);
    moment->setPen(QPen(Qt::red, 3));

    if (PMom != 0){
        moment->attach( plot );

        PLOTOBJECT var;
        var.itemPtr = moment;
        var.type    = PLType::LOAD;
        var.index   = 2;
        plotItemList.append(var);
    }

    //
    // Drawing Horizontal Force Arrow using QwtPlotShapeItem
    //
    QwtPlotShapeItem *arrow = new QwtPlotShapeItem();
    double forceArrowRatio = -P/MAX_H_FORCE;

    // Defining minimum size of the horizontal force arrow
    double forceArrowMin = 0.03;
    if (( forceArrowRatio < forceArrowMin ) && (forceArrowRatio > 0)) {
        forceArrowRatio = forceArrowMin;
    }
    else if (( forceArrowRatio > -forceArrowMin ) && (forceArrowRatio < 0)) {
        forceArrowRatio = -forceArrowMin;
    }

    QPen pen( Qt::black, 1 );
    pen.setJoinStyle( Qt::MiterJoin );
    arrow->setPen( pen );
    arrow->setBrush( Qt::red );

    // Horizontal Force Arrow dimensions
    double arrowThickness  =  3 * yScalar,
           arrowHead       = 10 * yScalar,
           arrowHeadLength = 15 * xScalar;

    if (forceArrowRatio < 0) {arrowHeadLength = -arrowHeadLength;}

    QPainterPath path;
    path.moveTo( pileCapCenter                                            , L1 + maxH                    );
    path.lineTo( pileCapCenter + arrowHeadLength                          , L1 + maxH + arrowHead/2      );
    path.lineTo( pileCapCenter + arrowHeadLength                          , L1 + maxH + arrowThickness/2 );
    path.lineTo( pileCapCenter + arrowHeadLength + forceArrowRatio*( W/2 ), L1 + maxH + arrowThickness/2 );
    path.lineTo( pileCapCenter + arrowHeadLength + forceArrowRatio*( W/2 ), L1 + maxH - arrowThickness/2 );
    path.lineTo( pileCapCenter + arrowHeadLength                          , L1 + maxH - arrowThickness/2 );
    path.lineTo( pileCapCenter + arrowHeadLength                          , L1 + maxH - arrowHead/2      );
    path.lineTo( pileCapCenter                                            , L1 + maxH                    );
    arrow->setShape( path );

    if (forceArrowRatio != 0){
        arrow->attach( plot );

        PLOTOBJECT var;
        var.itemPtr = arrow;
        var.type    = PLType::LOAD;
        var.index   = 0;
        plotItemList.append(var);
    }

    // Drawing Vertical Force Arrow using QwtPlotShapeItem
    QwtPlotShapeItem *arrowV = new QwtPlotShapeItem();

    double forceArrowRatioV =  PV/MAX_V_FORCE;
    double yLength          =  -depthOfLayer[3]/2;
    double arrowHeadLengthV =  15 * yScalar;
    double arrowHeadV       =  10 * xScalar;
    double arrowWidthV      =   3 * xScalar;


    if ( PV > 0.0 && forceArrowRatioV <  0.3 ) forceArrowRatio =  0.3;
    if ( PV < 0.0 && forceArrowRatioV > -0.3 ) forceArrowRatio = -0.3;

    arrowV->setPen( pen );
    arrowV->setBrush( Qt::red );

    QPainterPath pathV;
    if (forceArrowRatioV < 0) {
        pathV.moveTo( pileCapCenter + arrowWidthV/2, L1 + maxH);
        pathV.lineTo( pileCapCenter + arrowWidthV/2, L1 + maxH - yLength*forceArrowRatioV - arrowHeadLengthV);
        pathV.lineTo( pileCapCenter + arrowHeadV/2 , L1 + maxH - yLength*forceArrowRatioV - arrowHeadLengthV);
        pathV.lineTo( pileCapCenter                , L1 + maxH - yLength*forceArrowRatioV);
        pathV.lineTo( pileCapCenter - arrowHeadV/2 , L1 + maxH - yLength*forceArrowRatioV - arrowHeadLengthV );
        pathV.lineTo( pileCapCenter - arrowWidthV/2, L1 + maxH - yLength*forceArrowRatioV - arrowHeadLengthV);
        pathV.lineTo( pileCapCenter - arrowWidthV/2, L1 + maxH);
        pathV.lineTo( pileCapCenter + arrowWidthV/2, L1 + maxH);
    }
    if (forceArrowRatioV > 0) {
        pathV.moveTo( pileCapCenter , L1 + maxH);
        pathV.lineTo( pileCapCenter + arrowHeadV/2 , L1 + maxH + arrowHeadLengthV);
        pathV.lineTo( pileCapCenter + arrowWidthV/2, L1 + maxH + arrowHeadLengthV);
        pathV.lineTo( pileCapCenter + arrowWidthV/2, L1 + maxH + arrowHeadLengthV + yLength*forceArrowRatioV);
        pathV.lineTo( pileCapCenter - arrowWidthV/2, L1 + maxH + arrowHeadLengthV + yLength*forceArrowRatioV);
        pathV.lineTo( pileCapCenter - arrowWidthV/2, L1 + maxH + arrowHeadLengthV);
        pathV.lineTo( pileCapCenter - arrowHeadV/2 , L1 + maxH + arrowHeadLengthV);
        pathV.moveTo( pileCapCenter , L1 + maxH);
    }

    arrowV->setShape( pathV );
    //arrowV->setZ	( 3 );

    if (PV != 0){
        arrowV->attach( plot );

        PLOTOBJECT var2;
        var2.itemPtr = arrowV;
        var2.type    = PLType::LOAD;
        var2.index   = 1;
        plotItemList.append(var2);
    }

    // status info
    if (!mIsStable)
    {
        //
        // write "unstable system" at center of the system plot
        //

        QwtPlotTextLabel *warning = new QwtPlotTextLabel();

        QwtText title("unstable\nsystem\n\nanalysis\ndid not\nconverge");
        title.setFont(QFont(font().family(), 36));
        title.setColor(Qt::magenta);
        warning->setText(title);
        warning->setMargin(8);
        warning->attach(plot);

        PLOTOBJECT var;
        var.itemPtr = warning;
        var.type    = PLType::OTHER;
        var.index   = 0;
        plotItemList.append(var);

    }

    plot->replot();

    // we should not need this but since Qt5.11.0 we do need it or newly added piles wont refresh
    this->repaint();
}
