#include "systemplotqcp.h"
#include "systemplotsuper.h"

SystemPlotQCP::SystemPlotQCP(QWidget *parent) :
    SystemPlotSuper(parent)
{
    //
    // create a QCustomPlot
    //
    plot = new QCustomPlot(this);

    QGridLayout *lyt = new QGridLayout(this);
    lyt->addWidget(plot,0,0);
    lyt->setMargin(0);
    this->setLayout(lyt);

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

    //
    // default plot selection settings
    //
    activePileIdx = 0;
    activeLayerIdx = -1;
}

SystemPlotQCP::~SystemPlotQCP()
{
    delete plot;
}

void SystemPlotQCP::refresh()
{
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

    plot->autoAddPlottableToLegend();
    plot->legend->setVisible(true);

    QVector<double> zero(2,xbar-0.5*W);
    QVector<double> loc(2,0.0);
    loc[0] = -(H-L1); loc[1] = L1;

    plot->addGraph();
    plot->graph(0)->setData(zero,loc);
    plot->graph(0)->setPen(QPen(Qt::black,1));
    plot->graph(0)->removeFromLegend();

    // create layers

    plot->setCurrentLayer("soil");

    for (int iLayer=0; iLayer<MAXLAYERS; iLayer++) {

        QVector<double> x(5,0.0);
        QVector<double> y(5,0.0);

        x[0] = xbar - W/2.+shift(depthOfLayer[iLayer]);   y[0] = -depthOfLayer[iLayer];
        x[1] = xbar - W/2.+shift(depthOfLayer[iLayer+1]); y[1] = -depthOfLayer[iLayer+1];
        x[2] = xbar + W/2.+shift(depthOfLayer[iLayer+1]); y[2] = y[1];
        x[3] = xbar + W/2.+shift(depthOfLayer[iLayer]);   y[3] = y[0];
        x[4] = x[0];                                      y[4] = y[0];

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

    // ground water table

    plot->setCurrentLayer("groundwater");

    if (gwtDepth < (H-L1)) {
        QVector<double> x;
        QVector<double> y;

        double xl = xbar - W/2;
        double xr = xbar + W/2;

        double s = soilMotion.last();

        s = shift(gwtDepth);
        x.append(xl + s);         y.append(-gwtDepth);

        for (int i=0; i<=MAXLAYERS; i++)
        {
            if (gwtDepth <= depthOfLayer[i])
            {
                s = shift(depthOfLayer[i]);
                x.append(xl + s);   y.append(-depthOfLayer[i]);
            }
        }

        s = shift((H - L1));
        x.append(xl + s);         y.append(-(H - L1));
        x.append(xr + s);         y.append(-(H - L1));

        for (int i=MAXLAYERS; i>=0; i--)
        {
            if (gwtDepth <= depthOfLayer[i])
            {
                s = shift(depthOfLayer[i]);
                x.append(xr + s);   y.append(-depthOfLayer[i]);
            }
        }

        s = shift(gwtDepth);
        x.append(xr + s);          y.append(-gwtDepth);
        x.append(xl + s);          y.append(-gwtDepth);

        QCPCurve *water = new QCPCurve(plot->xAxis, plot->yAxis);
        water->setData(x,y);

        water->setPen(QPen(Qt::blue, 2));
        water->setBrush(QBrush(GROUND_WATER_BLUE));

        water->setName(QString("groundwater"));
    }

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

        QVector<double> x;
        QVector<double> y;

        double D = pileDiameter[pileIdx];


        if (m_pos.size() == numPiles)
        {
            // deformed pile

            x.clear();
            y.clear();

            for (int i=0; i<m_pos[pileIdx]->size(); i++)
            {
                x.append( xOffset[pileIdx]     + (*m_dispU[pileIdx])[i] + D/2.);
                y.append( (*m_pos[pileIdx])[i] + (*m_dispV[pileIdx])[i]       );
            }

            for (int i=m_pos[pileIdx]->size()-1; i>=0; i--)
            {
                x.append( xOffset[pileIdx]     + (*m_dispU[pileIdx])[i] - D/2.);
                y.append( (*m_pos[pileIdx])[i] + (*m_dispV[pileIdx])[i]       );
            }
        }
        else
        {
            // undeformed pile

            x.clear();
            y.clear();

            x.append(xOffset[pileIdx] - D/2.);      y.append(L1);
            x.append(xOffset[pileIdx] - D/2.);      y.append(-L2[pileIdx]);
            x.append(xOffset[pileIdx] + D/2.);      y.append(-L2[pileIdx]);
            x.append(xOffset[pileIdx] + D/2.);      y.append(L1);
            x.append(xOffset[pileIdx] - D/2.);      y.append(L1);
        }

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

    // add force to the plot

    if (ABS(P) > 0.0) {
        double force = 0.45*W*P/MAX_H_FORCE;

        // add the arrow:
        QCPItemLine *arrow = new QCPItemLine(plot);
        //plot->addItem(arrow);
        arrow->setPen(QPen(Qt::red, 3));
        arrow->start->setCoords(xbar, L1);
        arrow->end->setCoords(xbar + force, L1);
        arrow->setHead(QCPLineEnding::esSpikeArrow);
    }

    if (ABS(PV) > 0.0) {
        double force = 0.45*W*PV/MAX_V_FORCE;

        // add the arrow:
        QCPItemLine *arrow = new QCPItemLine(plot);
        //plot->addItem(arrow);
        arrow->setPen(QPen(Qt::red, 3));
        arrow->start->setCoords(xbar, L1 - force);
        arrow->end->setCoords(xbar, L1);
        arrow->setHead(QCPLineEnding::esSpikeArrow);
    }

    // status info
    if (!mIsStable)
    {
        QCPItemText *warning = new QCPItemText(plot);
        warning->position->setType(QCPItemPosition::ptAxisRectRatio);
        warning->position->setCoords(0.5,  0.5);
        warning->setPositionAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
        warning->setText("unstable\nsystem");
        warning->setFont(QFont(font().family(), 36));
        warning->setPadding(QMargins(8, 0, 0, 0));
    }

    // plot scaling options

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    plot->xAxis->setScaleRatio(plot->yAxis);
    plot->rescaleAxes();
    plot->replot();
}

/* **** SLOTS **** */

void SystemPlotQCP::on_plot_selectionChangedByUser(void)
{
    foreach (QCPAbstractPlottable * item, plot->selectedPlottables()) {

        QString name = item->name();
        if (name.length()<1) name = "X";

        int layerIdx = -1;
        int pileIdx  = -1;

        switch (name.at(0).unicode()) {
        case 'P':
        case 'p':
            if (name.toLower() == QString("pile cap")) break;

            pileIdx = name.mid(6,1).toInt() - 1;

            activePileIdx  = pileIdx;
            activeLayerIdx = -1;

            emit on_pileSelected(activePileIdx);

            break;

        case 'L':
        case 'l':
            //qDebug() << "LAYER: " << name;
            layerIdx = name.mid(7,1).toInt() - 1;

            activePileIdx  = -1;
            activeLayerIdx = layerIdx;

            emit on_soilLayerSelected(activeLayerIdx);

            break;

        case 'G':
        case 'g':
            //qDebug() << "LAYER: " << name;

            activePileIdx  = -1;
            activeLayerIdx = -1;

            emit on_groundWaterSelected();

            break;

        default:
            qDebug() << "WHAT IS THIS? " << name;
        }
    }
    this->refresh();
}
