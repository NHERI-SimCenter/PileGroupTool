#include "pilefeamodeler.h"

extern int getTzParam(double phi, double b, double sigV, double pEleLength, double *tult, double *z50);
extern int getQzParam(double phiDegree, double b, double sigV, double G, double *qult, double *z50);
extern int getPyParam(double pyDepth,
                      double gamma,
                      double phiDegree,
                      double b,
                      double pEleLength,
                      double puSwitch,
                      double kSwitch,
                      double gwtSwitch,
                      double *pult,
                      double *y50);

// OpenSees include files
#include <Node.h>
#include <ID.h>
#include <SP_Constraint.h>
#include <MP_Constraint.h>
#include <Domain.h>
#include <StandardStream.h>
#include <LinearCrdTransf3d.h>
#include <DispBeamColumn3d.h>
#include <PySimple1.h>
#include <TzSimple1.h>
#include <QzSimple1.h>
#include <ZeroLength.h>
#include <LegendreBeamIntegration.h>
#include <ElasticSection3d.h>
#include <LinearSeries.h>
#include <NodalLoad.h>
#include <LoadPattern.h>
#include <SimulationInformation.h>

#include <LoadControl.h>
#include <RCM.h>
#include <PlainNumberer.h>
#include <NewtonRaphson.h>
#include <CTestNormDispIncr.h>
#include <TransformationConstraintHandler.h>
#include <PenaltyConstraintHandler.h>
#include <BandGenLinSOE.h>
#include <BandGenLinLapackSolver.h>
#include <StaticAnalysis.h>
#include <AnalysisModel.h>

#include <soilmat.h>

PileFEAmodeler::PileFEAmodeler()
{
    // set default parameters
}

void PileFEAmodeler::updatePiles(QMap<QString, double> &pileInfo)
{

}

void PileFEAmodeler::updateLoad(double Px, double Py, double M)
{

}

void PileFEAmodeler::updateSoil(QVector<soilLayer> &layers)
{

}

void PileFEAmodeler::updateDisplacement(double)
{

}

void PileFEAmodeler::updateDispProfile(QVector<double> &)
{

}

void PileFEAmodeler::setAnalysisType(QString)
{

}

void PileFEAmodeler::doAnalysis()
{

}

int  PileFEAmodeler::getExitStatus()
{

}
