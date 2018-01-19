/////////////////////////////////////////////////////////////
//                                                         //
// Procedure to compute ultimate lateral resistance, p_u,  //
//  and displacement at 50% of lateral capacity, y50, for  //
//  p-y springs representing cohesionless soil.            //
//                                                         //
//   Created by:   Hyung-suk Shin                          //
//                 University of Washington                //
//   Modified by:  Chris McGann                            //
//                 Pedro Arduino                           //
//                 Peter Mackenzie-Helnwein                //
//                 University of Washington                //
//                                                         //
/////////////////////////////////////////////////////////////

#include <QDebug>

// references
//  American Petroleum Institute (API) (1987). Recommended Practice for Planning, Designing and
//   Constructing Fixed Offshore Platforms. API Recommended Practice 2A(RP-2A), Washington D.C,
//   17th edition.
//
// Brinch Hansen, J. (1961). “The ultimate resistance of rigid piles against transversal forces.”
//  Bulletin No. 12, Geoteknisk Institute, Copenhagen, 59.
//
//  Boulanger, R. W., Kutter, B. L., Brandenberg, S. J., Singh, P., and Chang, D. (2003). Pile 
//   Foundations in liquefied and laterally spreading ground during earthquakes: Centrifuge experiments
//   and analyses. Center for Geotechnical Modeling, University of California at Davis, Davis, CA.
//   Rep. UCD/CGM-03/01.
//
//  Reese, L.C. and Van Impe, W.F. (2001), Single Piles and Pile Groups Under Lateral Loading.
//    A.A. Balkema, Rotterdam, Netherlands.

#include <cmath>

double atanh(double x)
{
    return (log(1+x) - log(1-x))/2.0;
}

int
getPyParam(double pyDepth,
	    double sig, 
	    double phiDegree, 
	    double b, 
	    double pEleLength, 
        int puSwitch,
        int kSwitch,
        int gwtSwitch,
	    double *pult, 
	    double *y50) {

  //----------------------------------------------------------
  //  define ultimate lateral resistance, pult 
  //----------------------------------------------------------
  // qDebug() << pyDepth << " " << gamma << " " << phiDegree << " " << b << " " << pEleLength << " " << puSwitch << " " << kSwitch << " " << gwtSwitch;
  // pult is defined per API recommendations (Reese and Van Impe, 2001 or API, 1987) for puSwitch = 1
  // OR per the method of Brinch Hansen (1961) for puSwitch = 2
  
  double pi = 3.14159265358979;
  double phi = phiDegree*(pi/180.);
  double zbRatio = pyDepth/b;
  
  double pu;
  double A;

  // obtain loading-type coefficient A for given depth-to-diameter ratio zb
  A = 0.88 + 1.966 * exp(-0.55*zbRatio);

  // set default value(s)
  pu = 0.0;

  //-------API recommended method-------
  if (puSwitch == 1) {

    // define common terms
    double alpha = phi/2.;
    double beta = pi/4. + phi/2.;
    double K0 = 0.4;
    double Ka = pow(tan(pi/4. - phi/2.),2);

    // terms for Equation (3.44), Reese and Van Impe (2001)
    double  c1 = K0*tan(phi)*sin(beta)/(tan(beta-phi)*cos(alpha));
    double  c2 = tan(beta)/tan(beta-phi)*tan(beta)*tan(alpha);
    double  c3 = K0*tan(beta)*(tan(phi)*sin(beta)-tan(alpha));
    double  c4 = tan(beta)/tan(beta-phi)-Ka;

    // terms for Equation (3.45), Reese and Van Impe (2001)
    double  c5 = Ka*(pow(tan(beta),8)-1);
    double  c6  = K0*tan(phi)*pow(tan(beta),4);
    
    // Equation (3.44), Reese and Van Impe (2001)
    double  pst = sig*(pyDepth*(c1+c2+c3) + b*c4);
    
    // Equation (3.45), Reese and Van Impe (2001)
    double  psd = b*sig*(c5+c6);
    //qDebug() << "pst psd: " << pst << " " << psd;

    // pult is the lesser of pst and psd. At surface, an arbitrary value is defined
    pu = A*fmin(pst,psd);

    //-------Brinch Hansen method-------
  } else if (puSwitch == 2) {

    // pressure at ground surface
    double  Kqo = exp((pi/2.+phi)*tan(phi))*cos(phi)*tan(pi/4.+phi/2.)-exp(-(pi/2.-phi)*tan(phi))*cos(phi)*tan(pi/4.-phi/2.);
    double  Kco = (1/tan(phi))*(exp((pi/2. + phi)*tan(phi))*cos(phi)*tan(pi/4. + phi/2.) - 1);
    
    // pressure at great depth
    double  dcinf = 1.58 + 4.09*(pow(tan(phi),4));
    double  Nc    = (1/tan(phi))*(exp(pi*tan(phi)))*(pow(tan(pi/4. + phi/2.),2) - 1);
    double  Ko    = 1 - sin(phi);
    double  Kcinf = Nc*dcinf;
    double  Kqinf = Kcinf*Ko*tan(phi);

    // pressure at an arbitrary depth
    double  aq  = (Kqo/(Kqinf - Kqo))*(Ko*sin(phi)/sin(pi/4. + phi/2.));
    double  KqD = (Kqo + Kqinf*aq*zbRatio)/(1 + aq*zbRatio);

    // ultimate lateral resistance
    pu = sig*KqD*b;
  }

  // PySimple1 material formulated with pult as a force, not force/length, multiply by trib. length
  if (pu < 0.01) pu = 0.01;
  *pult = pu*pEleLength;

  //----------------------------------------------------------
  //  define displacement at 50% lateral capacity, y50
  //----------------------------------------------------------
  
  // values of y50 depend of the coefficent of subgrade reaction, k, which can be defined in several ways.
  //  for gwtSwitch = 1, k reflects soil above the groundwater table
  //  for gwtSwitch = 2, k reflects soil below the groundwater table
  //  a linear variation of k with depth is defined for kSwitch = 1 after API (1987)
  //  a parabolic variation of k with depth is defined for kSwitch = 2 after Boulanger et al. (2003)
  
  // API (1987) recommended subgrade modulus for given friction angle, values obtained from figure (approximate)

  double ck0, ck1, ck2;

  if (gwtSwitch == 1) {

      // subgrade modulus above the water table

      // expansion using SI units
      ck0 = -5739.582666;
      ck1 = -3857.636939;
      ck2 = 146.2629258;
  } else {

      // subgrade modulus below the water table

      // expansion using SI units
      ck0 = -15337.63982;
      ck1 = -1240.567342;
      ck2 =  66.60706369;
  }

  double  k_SIunits = ck0 + phiDegree*(ck1 + ck2*phiDegree);
  if (k_SIunits < 10.0) k_SIunits = 10.0;

  // define parabolic distribution of k with depth if desired (i.e. lin_par switch == 2)
  double  sigV = sig;
  
  if (sigV < 0.01) {
    sigV = 0.01;
  }
  
  if (kSwitch == 2) {
    // Equation (5-16), Boulanger et al. (2003)
    double cSigma =  pow(50/sigV,0.5);
    // Equation (5-15), Boulanger et al. (2003)
    k_SIunits = cSigma*k_SIunits;
  }
  
  // define y50 based on pult and subgrade modulus k
  
  // based on API (1987) recommendations, p-y curves are described using tanh functions.
  //  tcl does not have the atanh function, so must define this specifically
  
  //  i.e.  atanh(x) = 1/2*ln((1+x)/(1-x)), |x| < 1
  
  // when half of full resistance has been mobilized, p(y50)/pult = 0.5
  //double  x = 0.5;
  //double  atanh_value = 0.5*log((1+x)/(1-x));
  double  atanh_value = 0.5*log(3.0);
  
  // need to be careful at ground surface (don't want to divide by zero)
  if (pyDepth < 0.01) { pyDepth = 0.01; }

  // compute y50 (need to use pult in units of force/length, and also divide out the coeff. A)
  *y50  = 0.5*(pu/A)/(k_SIunits*pyDepth)*atanh_value;
  //qDebug() << *pult << " " << *y50;
  return 0;
}
