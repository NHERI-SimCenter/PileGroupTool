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
