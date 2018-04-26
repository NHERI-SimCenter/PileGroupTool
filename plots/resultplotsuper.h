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
    virtual void plotResults(QVector<QVector<double> *> &, QVector<QVector<double> *> &) = 0;
    virtual void updateSoil(QVector<double> &layerDepth);

signals:

public slots:

protected:
    QVector<double> depthOfLayer = QVector<double>(4, 0.0);
};

#endif // RESULTPLOTSUPER_H
