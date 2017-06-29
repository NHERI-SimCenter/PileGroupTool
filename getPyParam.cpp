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
	    double puSwitch, 
	    double kSwitch, 
	    double gwtSwitch, 
	    double *pult, 
	    double *y50) {

  //----------------------------------------------------------
  //  define ultimate lateral resistance, pult 
  //----------------------------------------------------------
   //  qDebug() << pyDepth << " " << gamma << " " << phiDegree << " " << b << " " << pEleLength << " " << puSwitch << " " << kSwitch << " " << gwtSwitch;
  // pult is defined per API recommendations (Reese and Van Impe, 2001 or API, 1987) for puSwitch = 1
  //  OR per the method of Brinch Hansen (1961) for puSwitch = 2
  
  double pi = 3.14159265358979;
  double phi = phiDegree*(pi/180.);
  double zbRatio = pyDepth/b;
  
  double pu;
  double A;

  // obtain loading-type coefficient A for given depth-to-diameter ratio zb
  //  ---> values are obtained from a figure and are therefore approximate
  double zb[42];
  zb[1] =   0;
  zb[2] =   0.1250;
  zb[3] =   0.2500;
  zb[4] =   0.3750;
  zb[5] =   0.5000;
  zb[6] =   0.6250;
  zb[7] =   0.7500;
  zb[8] =   0.8750;
  zb[9] =   1.0000;
  zb[10] =  1.1250;
  zb[11] =  1.2500;
  zb[12] =  1.3750;
  zb[13] =  1.5000;
  zb[14] =  1.6250;
  zb[15] =  1.7500;
  zb[16] =  1.8750;
  zb[17] =  2.0000;
  zb[18] =  2.1250;
  zb[19] =  2.2500;
  zb[20] =  2.3750;
  zb[21] =  2.5000;
  zb[22] =  2.6250;
  zb[23] =  2.7500;
  zb[24] =  2.8750;
  zb[25] =  3.0000;
  zb[26] =  3.1250;
  zb[27] =  3.2500;
  zb[28] =  3.3750;
  zb[29] =  3.5000;
  zb[30] =  3.6250;
  zb[31] =  3.7500;
  zb[32] =  3.8750;
  zb[33] =  4.0000;
  zb[34] =  4.1250;
  zb[35] =  4.2500;
  zb[36] =  4.3750;
  zb[37] =  4.5000;
  zb[38] =  4.6250;
  zb[39] =  4.7500;
  zb[40] =  4.8750;
  zb[41] =  5.0000;
  
  double As[42];
  As[1] =   2.8460;
  As[2] =   2.7105;
  As[3] =   2.6242;
  As[4] =   2.5257;
  As[5] =   2.4271;
  As[6] =   2.3409;
  As[7] =   2.2546;
  As[8] =   2.1437;
  As[9] =   2.0575;
  As[10] =  1.9589;
  As[11] =  1.8973;
  As[12] =  1.8111;
  As[13] =  1.7372;
  As[14] =  1.6632;
  As[15] =  1.5893;
  As[16] =  1.5277;
  As[17] =  1.4415;
  As[18] =  1.3799;
  As[19] =  1.3368;
  As[20] =  1.2690;
  As[21] =  1.2074;
  As[22] =  1.1581;
  As[23] =  1.1211;
  As[24] =  1.0780;
  As[25] =  1.0349;
  As[26] =  1.0164;
  As[27] =  0.9979;
  As[28] =  0.9733;
  As[29] =  0.9610;
  As[30] =  0.9487;
  As[31] =  0.9363;
  As[32] =  0.9117;
  As[33] =  0.8994;
  As[34] =  0.8994;
  As[35] =  0.8871;
  As[36] =  0.8871;
  As[37] =  0.8809;
  As[38] =  0.8809;
  As[39] =  0.8809;
  As[40] =  0.8809;
  As[41] =  0.8809;
  
  int dataNum = 41;
  
  // linear interpolation to define A for intermediate values of depth:diameter ratio
  if (zbRatio < zb[1])
    A = As[1];
  else if (zbRatio > zb[41])
    A = As[41];
  else {
    for (int i=1; i < dataNum; i++) {
      if ((zb[i] <= zbRatio)  && (zbRatio <= zb[i+1])) {
	A = (As[i+1]-As[i])/(zb[i+1]-zb[i]) * (zbRatio-zb[i]) + As[i];
      } else if (zbRatio >= 5) {
	A  = 0.88;
      }
    }
  }
  
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
    if (pst <= psd) {
      if (pyDepth == 0) 
	pu = 0.01; 
      else 
	pu = A*pst;
    } else {
      pu = A*psd;
    }

    // PySimple1 material formulated with pult as a force, not force/length, multiply by trib. length
    *pult = pu*pEleLength;
    //qDebug() << *pult << " " << pu;
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
    if (pyDepth == 0) { 
      pu = 0.01;
    } else {
      pu = sig*KqD*b;
    }
    
    // PySimple1 material formulated with pult as a force, not force/length, multiply by trib. length
    *pult = pu*pEleLength;
  }

  //----------------------------------------------------------
  //  define displacement at 50% lateral capacity, y50
  //----------------------------------------------------------
  
  // values of y50 depend of the coefficent of subgrade reaction, k, which can be defined in several ways.
  //  for gwtSwitch = 1, k reflects soil above the groundwater table
  //  for gwtSwitch = 2, k reflects soil below the groundwater table
  //  a linear variation of k with depth is defined for kSwitch = 1 after API (1987)
  //  a parabolic variation of k with depth is defined for kSwitch = 2 after Boulanger et al. (2003)
  
  // API (1987) recommended subgrade modulus for given friction angle, values obtained from figure (approximate)
  double ph[14];
  ph[1] = 28.8;  
  ph[2] = 29.5;  
  ph[3] = 30.0;  
  ph[4] = 31.0;   
  ph[5] = 32.0;    
  ph[6] = 33.0;
  ph[7] = 34.0;
  ph[8] = 35.0;
  ph[9] = 36.0;
  ph[10] = 37.0;
  ph[11] = 38.0;
  ph[12] = 39.0;
  ph[13] = 40.0;

  // subgrade modulus above the water table
  double k[14];
  if (gwtSwitch == 1) {
    // units of k are lb/in^3
    k[1] =   10;
    k[2] =   23;
    k[3] =   45;
    k[4] =   61;
    k[5] =   80;
    k[6] =   100;
    k[7] =   120;
    k[8] =   140;
    k[9] =   160;
    k[10] =  182;
    k[11] =  215;
    k[12] =  250;
    k[13] =  275;
    
    // subgrade modulus below the water table
  } else {
    // units of k are lb/in^3
    k[1] =   10;
    k[2] =   20;
    k[3] =   33;
    k[4] =   42;
    k[5] =   50;
    k[6] =   60;
    k[7] =   70;
    k[8] =   85;
    k[9] =   95;
    k[10] =  107;
    k[11] =  122;
    k[12] =  141;
    k[13] =  155;
  }
  
  dataNum = 13;
  // linear interpolation for values of phi not represented above
  double khat;
  if (phiDegree < ph[1])
    khat = k[1];
  else if (phiDegree > ph[13])
    khat = k[13];
  else {
    for (int i=1; i < dataNum; i++) {
      if ((ph[i] <= phiDegree ) && (phiDegree <= ph[i+1])) {
        khat = (k[i+1]-k[i])/(ph[i+1]-ph[i])*(phiDegree - ph[i]) + k[i];
      }
    }
  }
  
  // change units from (lb/in^3) to (kN/m^3)
  double  k_SIunits = khat*271.45;

  // define parabolic distribution of k with depth if desired (i.e. lin_par switch == 2)
  double  sigV = sig;
  
  if (sigV == 0) {
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
  double  x = 0.5;
  double  atanh_value = 0.5*log((1+x)/(1-x));
  
  // need to be careful at ground surface (don't want to divide by zero)
  if (pyDepth == 0.0) {
    pyDepth = 0.01;
  }
  

  // compute y50 (need to use pult in units of force/length, and also divide out the coeff. A)
  *y50  = 0.5*(pu/A)/(k_SIunits*pyDepth)*atanh(x);
 //qDebug() << *pult << " " << *y50;
  return 0;
}
