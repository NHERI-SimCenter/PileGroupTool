#ifndef RESULTPLOTSUPER_H
#define RESULTPLOTSUPER_H

#include <QWidget>
#include "pilegrouptool_parameters.h"

class ResultPlotSuper : public QWidget
{
    Q_OBJECT
public:
    explicit ResultPlotSuper(QWidget *parent = nullptr);
    virtual void plotResults(QVector<QVector<double> *> &, QVector<QVector<double> *> &) = 0;

signals:

public slots:
};

#endif // RESULTPLOTSUPER_H
