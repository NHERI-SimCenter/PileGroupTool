/////////////////////////////////////////////////////////////
//                                                         //
// Procedure to compute ultimate resistance, tult, and     //
//  displacement at 50% mobilization of tult, z50, for     //
//  use in t-z curves for cohesionless soil.               //
//                                                         //
//   Created by:  Chris McGann                             //
//                University of Washington                 //
//   Updated by:  Peter Mackenzie-Helnwein                 //
//                University of Washington                 //
//                                                         //
/////////////////////////////////////////////////////////////

#include <cmath>

int 
getTzParam(double phi, double b, double sigV, double pEleLength, double *tult, double *z50) {

  // references
  //  Mosher, R.L. (1984). “Load transfer criteria for numerical analysis of
  //   axial loaded piles in sand.” U.S. Army Engineering and Waterways
  //   Experimental Station, Automatic Data Processing Center, Vicksburg, Miss.
  //
  //  Kulhawy, F.H. (1991). "Drilled shaft foundations." Foundation engineering
  //   handbook, 2nd Ed., Chap 14, H.-Y. Fang ed., Van Nostrand Reinhold, New York
  
  double pi = 3.14159265358979;
  
  // Compute tult based on tult = Ko*sigV*pi*dia*tan(delta), where
  //   Ko    is coeff. of lateral earth pressure at rest, 
  //         taken as Ko = 0.4
  //   delta is interface friction between soil and pile,
  //         taken as delta = 0.8*phi to be representative of a 
  //         smooth precast concrete pile after Kulhawy (1991)
  double delta = 0.8*phi*pi/180.;
  
  // if z = 0 (ground surface) need to specify a small non-zero value of sigV
  if (sigV <= 0.0 ) {
    sigV = 0.001;
  }
  
  double tu = 0.4*sigV*pi*b*tan(delta);
  
  // TzSimple1 material formulated with tult as force, not stress, multiply by tributary length of pile
  *tult = tu*pEleLength;

/*
  // Mosher (1984) provides recommended initial tangents based on friction angle
  // values are in units of psf/in
  double kf[7];
  kf[1] =    6000.;
  kf[2] =   10000.;
  kf[3] =   10000.;
  kf[4] =   14000.;
  kf[5] =   14000.;
  kf[6] =   18000.;
  
  double fric[7];
  fric[1] = 28.;
  fric[2] = 31.;
  fric[3] = 32.;
  fric[4] = 34.;
  fric[5] = 35.;
  fric[6] = 38.;
  
  int dataNum = 6;
  
  // determine kf for input value of phi, linear interpolation for intermediate values
  double k;
  if (phi < fric[1]) {
    k = kf[1];
  } else if (phi > fric[6]) {
    k = kf[6];
  } else {
    for (int i=1; i < dataNum; i++) {
      if ( (fric[i] <= phi) && (phi <= fric[i+1]) )  {
	k = (kf[i+1] - kf[i]) / (fric[i+1] - fric[i])*(phi - fric[i]) + kf[i];
      } 
    }
  }
  
  // need to convert kf to units of kN/m^3
  double kSIunits = k*1.885;
*/

  // better use a regression in kN/m^3 (Peter Mackenzie-Helnwein, 2018)
  double kSIunits = 2304. * phi - 53408.;
  if (kSIunits < 1.e-4) kSIunits = 1.e-4;
  
  // based on a t-z curve of the shape recommended by Mosher (1984), z50 = tult/kf
  *z50 = *tult/kSIunits;
  
  return 0;
}
