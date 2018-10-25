/* ********************************************************************** *
 *
 * ResultPlotSuper is a virtual class used to setup and operate
 *    alternative implementations of ResultPlot.  At this point,
 *    the following derived classes provide the desired functionality:
 *
 *    1) ResultPlotQCP : QCustomPlot
 *    2) ResultPlotQwt : Qwt
 *
 * ********************************************************************** */

/*!
 * \class ResultPlotSuper
 * \brief The ResultPlotSuper class is an abstract class implementing simple line plots.
 * \since 2.0
 * \ingroup Plotting
 *
 * This class provides library-independent data exchange and data management for result
 * plots.
 *
 * Any library-dependent functionality needs to be implemented by a derived class.
 * The derived class must implement the \method refresh method.
 *
 *  \note Implementations exist for both the Qwt and the QCustomPlot libraries.
 *
 *  \sa ResultPlotQwt, ResultPlotQCP
 */


#include "resultplotsuper.h"

ResultPlotSuper::ResultPlotSuper(QWidget *parent) : QWidget(parent)
{

}

void ResultPlotSuper::updateSoil(QVector<double> &layerDepth)
{
    int cnt = layerDepth.size();

    bool upToDate = true;

    if (cnt > 0) {
        if (depthOfLayer[0] != layerDepth[0] )
        {
            depthOfLayer[0] = layerDepth[0];
            upToDate = false;
        }
    } else {
        depthOfLayer[1] = 0.00;
        upToDate = false;
    }

    if (cnt > 1) {
        if (depthOfLayer[1] != layerDepth[1] )
        {
            depthOfLayer[1] = layerDepth[1];
            upToDate = false;
        }
    } else {
        depthOfLayer[1] = depthOfLayer[0];
        upToDate = false;
    }

    if (cnt > 2 ) {
        if (depthOfLayer[2] != layerDepth[2] )
        {
            depthOfLayer[2] = layerDepth[2];
            upToDate = false;
        }
    } else  {
        depthOfLayer[2] = depthOfLayer[1];
        upToDate = false;
    }

    if (cnt > 3) {
        if (depthOfLayer[3] != layerDepth[3] )
        {
            depthOfLayer[3] = layerDepth[3];
            upToDate = false;
        }
    } else {
        depthOfLayer[3] = depthOfLayer[2];
        upToDate = false;
    }
}

void ResultPlotSuper::plotResults(QVector<QVector<double> *> &pos,
                                  QVector<QVector<double> *> &data)
{
    this->plotResults(pos, data, "", "");
}

void ResultPlotSuper::plotResults(QVector<QVector<double> *> &pos,
                                  QVector<QVector<double> *> &data,
                                  const QString &posLabel,
                                  const QString &dataLabel)
{
    m_posLabel  = posLabel;
    m_dataLabel = dataLabel;

    m_pos.clear();
    m_data.clear();

    foreach(QVector<double> *ptr, pos)
    {
        m_pos.append(*ptr);
    }

    foreach(QVector<double> *ptr, data)
    {
        m_data.append(*ptr);
    }

    this->refresh();
}
