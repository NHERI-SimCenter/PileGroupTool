/////////////////////////////////////////////////////////////
//                                                         //
// Procedure to compute ultimate tip resistance, qult, and //
//  displacement at 50% mobilization of qult, z50, for     //
//  use in q-z curves for cohesionless soil.               //
//                                                         //
//   Created by:  Chris McGann                             //
//                Pedro Arduino                            //
//                University of Washington                 //
//                                                         //
/////////////////////////////////////////////////////////////

#include <QDebug>

// references
//  Meyerhof G.G. (1976). "Bearing capacity and settlement of pile foundations." 
//   J. Geotech. Eng. Div., ASCE, 102(3), 195-228.
//
//  Vijayvergiya, V.N. (1977). “Load-movement characteristics of piles.”
//   Proc., Ports 77 Conf., ASCE, New York.
//
//  Kulhawy, F.H. ad Mayne, P.W. (1990). Manual on Estimating Soil Properties for 
//   Foundation Design. Electrical Power Research Institute. EPRI EL-6800, 
//   Project 1493-6 Final Report.

#include <cmath>

int getQzParam(double phiDegree, double b, double sigV, double G, double *qult, double *z50) {

   // qDebug() << phiDegree << " " << b << " " << sigV << " " << G;
  // define required constants; pi, atmospheric pressure (kPa), pa, and coeff. of lat earth pressure, Ko
  double pi = 3.14159265358979;
  double pa = 101.;
  double Ko = 1 - sin(phiDegree*pi/180);
  
  // ultimate tip pressure can be computed by qult = Nq*sigV after Meyerhof (1976)
  //  where Nq is a bearing capacity factor, phi is friction angle, and sigV is eff. overburden
  //  stress at the pile tip.
  double  phi = phiDegree*pi/180;
  
  // rigidity index
  double  Ir =G/(sigV*tan(phi));

  //qDebug() << Ir << " " << Ko;
  // bearing capacity factor
  double Nq  = (1+2*Ko)*(1/(3.-sin(phi)))*exp(pi/2.-phi)*(pow(tan(pi/4.+phi/2.),2))*(pow(Ir,(4*sin(phi))/(3*(1+sin(phi)))));
  // tip resistance
  double qu = Nq*sigV;
  // QzSimple1 material formulated with qult as force, not stress, multiply by area of pile tip
  *qult = qu*pi*pow(b,2)/4.;
  //qDebug() << Nq << " " << sigV << "  " <<  qu << " " << pi << " " << b;

  // the q-z curve of Vijayvergiya (1977) has the form, q(z) = qult*(z/zc)^(1/3)
  //  where zc is critical tip deflection given as ranging from 3-9% of the
  //  pile diameter at the tip.  
  
  // assume zc is 5% of pile diameter
  double zc = 0.05*b;
  
  // based on Vijayvergiya (1977) curve, z50 = 0.125*zc
  *z50 = 0.125*zc;
  
  return 0;
}
