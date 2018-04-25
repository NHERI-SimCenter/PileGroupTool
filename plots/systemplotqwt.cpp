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
    QwtPicker *picker = new QwtPicker(plot -> canvas());
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
    emptyObj.itemPtr = NULL;
    emptyObj.type    = PLType::NONE;
    emptyObj.index   = -1;

    if ( plot == NULL )
        return emptyObj;

    // translate pos into the plot coordinates
    double coords[ QwtPlot::axisCnt ];
    coords[ QwtPlot::xBottom ] = plot->canvasMap( QwtPlot::xBottom ).invTransform( pos.x() );
    coords[ QwtPlot::xTop ]    = plot->canvasMap( QwtPlot::xTop ).invTransform( pos.x() );
    coords[ QwtPlot::yLeft ]   = plot->canvasMap( QwtPlot::yLeft ).invTransform( pos.y() );
    coords[ QwtPlot::yRight ]  = plot->canvasMap( QwtPlot::yRight ).invTransform( pos.y() );

    for ( int i = plotItemList.size() - 1; i >= 0; i-- )
    {
        PLOTOBJECT obj = plotItemList[i];
        QwtPlotItem *item = obj.itemPtr;
        if ( item->isVisible() && item->rtti() == QwtPlotItem::Rtti_PlotCurve )
        {
            double dist;

            QwtPlotCurve *curveItem = static_cast<QwtPlotCurve *>( item );
            const QPointF p( coords[ item->xAxis() ], coords[ item->yAxis() ] );

            if ( curveItem->boundingRect().contains( p ) || true )
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
                        QPointF r2 = pos - x1;
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
    qWarning() << "picker appended " << pos;

    SystemPlotQwt::refresh();

    double coords[ QwtPlot::axisCnt ];
    coords[ QwtPlot::xBottom ] = plot->canvasMap( QwtPlot::xBottom ).invTransform( pos.x() );
    coords[ QwtPlot::xTop ]    = plot->canvasMap( QwtPlot::xTop ).invTransform( pos.x() );
    coords[ QwtPlot::yLeft ]   = plot->canvasMap( QwtPlot::yLeft ).invTransform( pos.y() );
    coords[ QwtPlot::yRight ]  = plot->canvasMap( QwtPlot::yRight ).invTransform( pos.y() );

    PLOTOBJECT    obj = itemAt(pos);
    QwtPlotItem *item = obj.itemPtr;

    if ( item )
    {
        /*if ( item->rtti() == QwtPlotItem::Rtti_PlotShape )
        {
            QwtPlotShapeItem *theShape = static_cast<QwtPlotShapeItem *>(item);
            theShape->setPen(Qt::red, 3);
            QBrush brush = theShape->brush();
            QColor color = brush.color();
            color.setAlpha(64);
            brush.setColor(color);
            theShape->setBrush(brush);
        }*/

        plot->replot();

        switch (obj.type) {
        case PLType::PILE:
            emit on_pileSelected(obj.index);
            break;
        case PLType::SOIL:
            emit on_soilLayerSelected(obj.index);
            break;
        case PLType::WATER:
            emit on_groundWaterSelected();
            break;
        }
    }
}


void SystemPlotQwt::refresh()
{  
    //qDebug() << "entering SystemPlotQwt::refresh()" << QTime::currentTime();
    foreach (PLOTOBJECT item, plotItemList) {
        item.itemPtr->detach();
        delete item.itemPtr;
        //item.itemPtr = NULL;
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

    //
    // Plot Legend
    //
    plot->insertLegend( new QwtLegend(), QwtPlot::BottomLegend );

    // Adjust x-axis to match Ground Layer Width
    plot->setAxisScale( QwtPlot::xBottom, xbar - W/2, xbar + W/2 );
    // Adjust y-axis to match Ground Layer Depth and slightly above pilecap
    double heightAbovePileCap = 1;
    plot->setAxisScale( QwtPlot::yLeft, -depthOfLayer[3], L1 + maxH + heightAbovePileCap );

    //
    // Plot Ground Water Table
    //

    //plot->setCurrentLayer("groundwater");

    if (gwtDepth < (H-L1)) {
        QPolygonF(groundwaterCorners);
        groundwaterCorners << QPointF(xbar - W/2, -gwtDepth)
                           << QPointF(xbar - W/2, -(H - L1))
                           << QPointF(xbar + W/2, -(H - L1))
                           << QPointF(xbar + W/2, -gwtDepth)
                           << QPointF(xbar - W/2, -gwtDepth);

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
        groundCorners << QPointF(xbar - W/2, -depthOfLayer[iLayer])
                      << QPointF(xbar - W/2, -depthOfLayer[iLayer+1])
                      << QPointF(xbar + W/2, -depthOfLayer[iLayer+1])
                      << QPointF(xbar + W/2, -depthOfLayer[iLayer])
                      << QPointF(xbar - W/2, -depthOfLayer[iLayer]);

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
    // Plot PileCaps
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



#if 0
    // plot the pile cap

    plot->setCurrentLayer("piles");

    QVector<double> x(5,0.0);
    QVector<double> y(5,0.0);
QwtLegend
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

    double pileCapCenter  = 0.5 * (minX0 + maxX0),
           arrowThickness = 0.1,
           arrowHead = 0.4,
           arrowHeadLength = 0.5;

    if (forceArrowRatio < 0) {arrowHeadLength = -arrowHeadLength;}

    QPainterPath path;
    path.moveTo( pileCapCenter                                            , L1 + maxH                  );
    path.lineTo( pileCapCenter + arrowHeadLength                          , L1 + maxH + arrowHead      );
    path.lineTo( pileCapCenter + arrowHeadLength                          , L1 + maxH + arrowThickness );
    path.lineTo( pileCapCenter + arrowHeadLength + forceArrowRatio*( W/2 ), L1 + maxH + arrowThickness );
    path.lineTo( pileCapCenter + arrowHeadLength + forceArrowRatio*( W/2 ), L1 + maxH - arrowThickness );
    path.lineTo( pileCapCenter + arrowHeadLength                          , L1 + maxH - arrowThickness );
    path.lineTo( pileCapCenter + arrowHeadLength                          , L1 + maxH - arrowHead      );
    path.lineTo( pileCapCenter                                            , L1 + maxH                  );
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
    /*
    QwtPlotShapeItem *arrowV = new QwtPlotShapeItem();

    double forceArrowRatioV = PV/MAX_V_FORCE;

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

    // status info
    if (!mIsStable)
    {
        //
        // TODO: write "unstable system" at center of the system plot
        //

        // this is the code from QCP

        QwtPlotTextLabel *warning = new QwtPlotTextLabel();

        QwtText title("unstable\nsystem");
        title.setFont(QFont(font().family(), 36));
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

}
