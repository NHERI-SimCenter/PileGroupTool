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
