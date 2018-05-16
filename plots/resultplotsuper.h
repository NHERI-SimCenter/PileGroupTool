#ifndef RESULTPLOTSUPER_H
#define RESULTPLOTSUPER_H

#include <QWidget>
#include <QVector>
#include "pilegrouptool_parameters.h"

class ResultPlotSuper : public QWidget
{
    Q_OBJECT
public:
    explicit ResultPlotSuper(QWidget *parent = nullptr);
    virtual void plotResults(QVector<QVector<double> *> &, QVector<QVector<double> *> &);
    virtual void updateSoil(QVector<double> &layerDepth);
    virtual void refresh(void) = 0;

signals:

public slots:

protected:
    QVector<double> depthOfLayer = QVector<double>(4, 0.0);
    QVector<QVector<double> > m_pos;   // lists of depth of points
    QVector<QVector<double> > m_data;  // lists of associated results at points
};

#endif // RESULTPLOTSUPER_H
