#ifndef SYSTEMPLOTQWT_H
#define SYSTEMPLOTQWT_H

#include <QWidget>
#include "pilegrouptool_parameters.h"
#include "systemplotsuper.h"

class QwtPlot;
class QwtPlotGrid;
class QwtPlotItem;
class QwtPlotShapeItem;

enum class PLType { PILE, SOIL, WATER, CAP, LOAD, OTHER, NONE };

struct PLOTOBJECT {
    QwtPlotItem *itemPtr;   // pointer to the PlotItem
    PLType       type;      // object type identifier
    int          index;     // pile index OR layer index
};

namespace Ui {
class SystemPlotQwt;
}

class SystemPlotQwt : public SystemPlotSuper
{
    Q_OBJECT

public:
    explicit SystemPlotQwt(QWidget *parent = 0);
    ~SystemPlotQwt();

    void refresh();

    PLOTOBJECT itemAt( const QPoint& pos ) const;


public slots:
    void on_picker_appended (const QPoint &pos);


private:

signals:
    void on_pileSelected(int );
    void on_soilLayerSelected(int );
    void on_groundWaterSelected(void);

protected:
    QwtPlot *plot;
    QwtPlotGrid *grid;

    QList<PLOTOBJECT> plotItemList;

};

#endif // SYSTEMPLOTQWT_H
