#include "GeoPoint.hpp"

// This file was adapted from geocent.[hc] from proj:

/***************************************************************************/
/* RSC IDENTIFIER:  GEOCENTRIC
 *
 * ABSTRACT
 *
 *    This component provides conversions between Geodetic coordinates (latitude,
 *    longitude in radians and height in meters) and Geocentric coordinates
 *    (X, Y, Z) in meters.
 *
 * ERROR HANDLING
 *
 *    This component checks parameters for valid values.  If an invalid value
 *    is found, the error code is combined with the current error code using 
 *    the bitwise or.  This combining allows multiple error codes to be
 *    returned. The possible error codes are:
 *
 *      GEOCENT_NO_ERROR        : No errors occurred in function
 *      GEOCENT_LAT_ERROR       : Latitude out of valid range
 *                                 (-90 to 90 degrees)
 *      GEOCENT_LON_ERROR       : Longitude out of valid range
 *                                 (-180 to 360 degrees)
 *      GEOCENT_A_ERROR         : Semi-major axis less than or equal to zero
 *      GEOCENT_B_ERROR         : Semi-minor axis less than or equal to zero
 *      GEOCENT_A_LESS_B_ERROR  : Semi-major axis less than semi-minor axis
 *
 *
 * REUSE NOTES
 *
 *    GEOCENTRIC is intended for reuse by any application that performs
 *    coordinate conversions between geodetic coordinates and geocentric
 *    coordinates.
 *    
 *
 * REFERENCES
 *    
 *    An Improved Algorithm for Geocentric to Geodetic Coordinate Conversion,
 *    Ralph Toms, February 1996  UCRL-JC-123138.
 *    
 *    Further information on GEOCENTRIC can be found in the Reuse Manual.
 *
 *    GEOCENTRIC originated from : U.S. Army Topographic Engineering Center
 *                                 Geospatial Information Division
 *                                 7701 Telegraph Road
 *                                 Alexandria, VA  22310-3864
 *
 * LICENSES
 *
 *    None apply to this component.
 *
 * RESTRICTIONS
 *
 *    GEOCENTRIC has no restrictions.
 *
 * ENVIRONMENT
 *
 *    GEOCENTRIC was tested and certified in the following environments:
 *
 *    1. Solaris 2.5 with GCC version 2.8.1
 *    2. Windows 95 with MS Visual C++ version 6
 *
 * MODIFICATIONS
 *
 *    Date              Description
 *    ----              -----------
 *
 *
 */


/***************************************************************************/
/*
 *                              DEFINES
 */
#define GEOCENT_NO_ERROR        0x0000
#define GEOCENT_LAT_ERROR       0x0001
#define GEOCENT_LON_ERROR       0x0002
#define GEOCENT_A_ERROR         0x0004
#define GEOCENT_B_ERROR         0x0008
#define GEOCENT_A_LESS_B_ERROR  0x0010

// Earth's equatorial radius in meters (WGS84)
#define GEOCENT_A 6378137.0

// Earth's polar radius in meters (WGS84)
#define GEOCENT_B 6356752.314245

#define PI         3.14159265358979323e0
#define PI_OVER_2  (PI / 2.0e0)
#define FALSE      0
#define TRUE       1
#define COS_67P5   0.38268343236508977  /* cosine of 67.5 degrees */
#define AD_C       1.0026000            /* Toms region 1 constant */

typedef struct 
{
    double Geocent_a;        /* Semi-major axis of ellipsoid in meters */
    double Geocent_b;        /* Semi-minor axis of ellipsoid           */
    double Geocent_a2;       /* Square of semi-major axis */
    double Geocent_b2;       /* Square of semi-minor axis */
    double Geocent_e2;       /* Eccentricity squared  */
    double Geocent_ep2;      /* 2nd eccentricity squared */
} GeocentricInfo;

static
const GeocentricInfo pj_wgs84 = {
  .Geocent_a = GEOCENT_A,
  .Geocent_b = GEOCENT_B,
  .Geocent_a2 = GEOCENT_A * GEOCENT_A,
  .Geocent_b2 = GEOCENT_B * GEOCENT_B,
  .Geocent_e2 = ((GEOCENT_A * GEOCENT_A) - (GEOCENT_B * GEOCENT_B)) / (GEOCENT_A * GEOCENT_A),
  .Geocent_ep2 = ((GEOCENT_A * GEOCENT_A) - (GEOCENT_B * GEOCENT_B)) / (GEOCENT_B * GEOCENT_B)
};

/*
 * The function Convert_Geodetic_To_Geocentric converts geodetic coordinates
 * (latitude, longitude, and height) to geocentric coordinates (X, Y, Z),
 * according to the current ellipsoid parameters.
 *
 *    Latitude  : Geodetic latitude in radians                     (input)
 *    Longitude : Geodetic longitude in radians                    (input)
 *    Height    : Geodetic height, in meters                       (input)
 *    X         : Calculated Geocentric X coordinate, in meters.   (output)
 *    Y         : Calculated Geocentric Y coordinate, in meters.   (output)
 *    Z         : Calculated Geocentric Z coordinate, in meters.   (output)
 *
 */

static 
long pj_Convert_Geodetic_To_Geocentric (const GeocentricInfo *gi,
                                        const double Latitude,
                                        const double Longitude,
                                        const double Height,
                                        double *X,
                                        double *Y,
                                        double *Z) 
{ /* BEGIN Convert_Geodetic_To_Geocentric */
/*
 * The function Convert_Geodetic_To_Geocentric converts geodetic coordinates
 * (latitude, longitude, and height) to geocentric coordinates (X, Y, Z),
 * according to the current ellipsoid parameters.
 *
 *    Latitude  : Geodetic latitude in radians                     (input)
 *    Longitude : Geodetic longitude in radians                    (input)
 *    Height    : Geodetic height, in meters                       (input)
 *    X         : Calculated Geocentric X coordinate, in meters    (output)
 *    Y         : Calculated Geocentric Y coordinate, in meters    (output)
 *    Z         : Calculated Geocentric Z coordinate, in meters    (output)
 *
 */
  long Error_Code = GEOCENT_NO_ERROR;
  double Rn;            /*  Earth radius at location  */
  double Sin_Lat;       /*  sin(Latitude)  */
  double Sin2_Lat;      /*  Square of sin(Latitude)  */
  double Cos_Lat;       /*  cos(Latitude)  */
  double Latitude_ = Latitude;
  double Longitude_ = Longitude;

  /*
  ** Don't blow up if Latitude is just a little out of the value
  ** range as it may just be a rounding issue.  Also removed longitude
  ** test, it should be wrapped by cos() and sin().  NFW for PROJ.4, Sep/2001.
  */
  if( Latitude_ < -PI_OVER_2 && Latitude_ > -1.001 * PI_OVER_2 )
      Latitude_ = -PI_OVER_2;
  else if( Latitude_ > PI_OVER_2 && Latitude_ < 1.001 * PI_OVER_2 )
      Latitude_ = PI_OVER_2;
  else if ((Latitude_ < -PI_OVER_2) || (Latitude_ > PI_OVER_2))
  { /* Latitude_ out of range */
    Error_Code |= GEOCENT_LAT_ERROR;
  }

  if (!Error_Code)
  { /* no errors */
    if (Longitude_ > PI)
      Longitude_ -= (2*PI);
    Sin_Lat = sin(Latitude_);
    Cos_Lat = cos(Latitude_);
    Sin2_Lat = Sin_Lat * Sin_Lat;
    Rn = gi->Geocent_a / (sqrt(1.0e0 - gi->Geocent_e2 * Sin2_Lat));
    *X = (Rn + Height) * Cos_Lat * cos(Longitude_);
    *Y = (Rn + Height) * Cos_Lat * sin(Longitude_);
    *Z = ((Rn * (1 - gi->Geocent_e2)) + Height) * Sin_Lat;

  }
  return (Error_Code);
} /* END OF Convert_Geodetic_To_Geocentric */

/*
 * The function Convert_Geocentric_To_Geodetic converts geocentric
 * coordinates (X, Y, Z) to geodetic coordinates (latitude, longitude, 
 * and height), according to the current ellipsoid parameters.
 *
 *    X         : Geocentric X coordinate, in meters.         (input)
 *    Y         : Geocentric Y coordinate, in meters.         (input)
 *    Z         : Geocentric Z coordinate, in meters.         (input)
 *    Latitude  : Calculated latitude value in radians.       (output)
 *    Longitude : Calculated longitude value in radians.      (output)
 *    Height    : Calculated height value, in meters.         (output)
 */

#define USE_ITERATIVE_METHOD

static
void pj_Convert_Geocentric_To_Geodetic (const GeocentricInfo *gi,
                                        const double X,
                                        const double Y, 
                                        const double Z,
                                        double *Latitude,
                                        double *Longitude,
                                        double *Height)
{ /* BEGIN Convert_Geocentric_To_Geodetic */
#if !defined(USE_ITERATIVE_METHOD)
/*
 * The method used here is derived from 'An Improved Algorithm for
 * Geocentric to Geodetic Coordinate Conversion', by Ralph Toms, Feb 1996
 */

/* Note: Variable names follow the notation used in Toms, Feb 1996 */

    double W;        /* distance from Z axis */
    double W2;       /* square of distance from Z axis */
    double T0;       /* initial estimate of vertical component */
    double T1;       /* corrected estimate of vertical component */
    double S0;       /* initial estimate of horizontal component */
    double S1;       /* corrected estimate of horizontal component */
    double Sin_B0;   /* sin(B0), B0 is estimate of Bowring aux variable */
    double Sin3_B0;  /* cube of sin(B0) */
    double Cos_B0;   /* cos(B0) */
    double Sin_p1;   /* sin(phi1), phi1 is estimated latitude */
    double Cos_p1;   /* cos(phi1) */
    double Rn;       /* Earth radius at location */
    double Sum;      /* numerator of cos(phi1) */
    int At_Pole;     /* indicates location is in polar region */

    At_Pole = FALSE;
    if (X != 0.0)
    {
        *Longitude = atan2(Y,X);
    }
    else
    {
        if (Y > 0)
        {
            *Longitude = PI_OVER_2;
        }
        else if (Y < 0)
        {
            *Longitude = -PI_OVER_2;
        }
        else
        {
            At_Pole = TRUE;
            *Longitude = 0.0;
            if (Z > 0.0)
            {  /* north pole */
                *Latitude = PI_OVER_2;
            }
            else if (Z < 0.0)
            {  /* south pole */
                *Latitude = -PI_OVER_2;
            }
            else
            {  /* center of earth */
                *Latitude = PI_OVER_2;
                *Height = -Geocent_b;
                return;
            } 
        }
    }
    W2 = X*X + Y*Y;
    W = sqrt(W2);
    T0 = Z * AD_C;
    S0 = sqrt(T0 * T0 + W2);
    Sin_B0 = T0 / S0;
    Cos_B0 = W / S0;
    Sin3_B0 = Sin_B0 * Sin_B0 * Sin_B0;
    T1 = Z + gi->Geocent_b * gi->Geocent_ep2 * Sin3_B0;
    Sum = W - gi->Geocent_a * gi->Geocent_e2 * Cos_B0 * Cos_B0 * Cos_B0;
    S1 = sqrt(T1*T1 + Sum * Sum);
    Sin_p1 = T1 / S1;
    Cos_p1 = Sum / S1;
    Rn = gi->Geocent_a / sqrt(1.0 - gi->Geocent_e2 * Sin_p1 * Sin_p1);
    if (Cos_p1 >= COS_67P5)
    {
        *Height = W / Cos_p1 - Rn;
    }
    else if (Cos_p1 <= -COS_67P5)
    {
        *Height = W / -Cos_p1 - Rn;
    }
    else
    {
        *Height = Z / Sin_p1 + Rn * (gi->Geocent_e2 - 1.0);
    }
    if (At_Pole == FALSE)
    {
        *Latitude = atan(Sin_p1 / Cos_p1);
    }
#else /* defined(USE_ITERATIVE_METHOD) */
/*
* Reference...
* ============
* Wenzel, H.-G.(1985): Hochauflösende Kugelfunktionsmodelle für
* das Gravitationspotential der Erde. Wiss. Arb. Univ. Hannover
* Nr. 137, p. 130-131.

* Programmed by GGA- Leibniz-Institute of Applied Geophysics
*               Stilleweg 2
*               D-30655 Hannover
*               Federal Republic of Germany
*               Internet: www.gga-hannover.de
*
*               Hannover, March 1999, April 2004.
*               see also: comments in statements
* remarks:
* Mathematically exact and because of symmetry of rotation-ellipsoid,
* each point (X,Y,Z) has at least two solutions (Latitude1,Longitude1,Height1) and
* (Latitude2,Longitude2,Height2). Is point=(0.,0.,Z) (P=0.), so you get even
* four solutions,	every two symmetrical to the semi-minor axis.
* Here Height1 and Height2 have at least a difference in order of
* radius of curvature (e.g. (0,0,b)=> (90.,0.,0.) or (-90.,0.,-2b);
* (a+100.)*(sqrt(2.)/2.,sqrt(2.)/2.,0.) => (0.,45.,100.) or
* (0.,225.,-(2a+100.))).
* The algorithm always computes (Latitude,Longitude) with smallest |Height|.
* For normal computations, that means |Height|<10000.m, algorithm normally
* converges after to 2-3 steps!!!
* But if |Height| has the amount of length of ellipsoid's axis
* (e.g. -6300000.m),	algorithm needs about 15 steps.
*/

/* local definitions and variables */
/* end-criterium of loop, accuracy of sin(Latitude) */
#define genau   1.E-12
#define genau2  (genau*genau)
#define maxiter 30

    double P;        /* distance between semi-minor axis and location */
    double RR;       /* distance between center and location */
    double CT;       /* sin of geocentric latitude */
    double ST;       /* cos of geocentric latitude */
    double RX;
    double RK;
    double RN;       /* Earth radius at location */
    double CPHI0;    /* cos of start or old geodetic latitude in iterations */
    double SPHI0;    /* sin of start or old geodetic latitude in iterations */
    double CPHI;     /* cos of searched geodetic latitude */
    double SPHI;     /* sin of searched geodetic latitude */
    double SDPHI;    /* end-criterium: addition-theorem of sin(Latitude(iter)-Latitude(iter-1)) */
    int iter;        /* # of continuous iteration, max. 30 is always enough (s.a.) */

    P = sqrt(X*X+Y*Y);
    RR = sqrt(X*X+Y*Y+Z*Z);

/*	special cases for latitude and longitude */
    if (P/gi->Geocent_a < genau) {

/*  special case, if P=0. (X=0., Y=0.) */
	*Longitude = 0.;

/*  if (X,Y,Z)=(0.,0.,0.) then Height becomes semi-minor axis
 *  of ellipsoid (=center of mass), Latitude becomes PI/2 */
        if (RR/gi->Geocent_a < genau) {
            *Latitude = PI_OVER_2;
            *Height   = -gi->Geocent_b;
            return ;

        }
    }
    else {
/*  ellipsoidal (geodetic) longitude
 *  interval: -PI < Longitude <= +PI */
        *Longitude=atan2(Y,X);
    }

/* --------------------------------------------------------------
 * Following iterative algorithm was developed by
 * "Institut für Erdmessung", University of Hannover, July 1988.
 * Internet: www.ife.uni-hannover.de
 * Iterative computation of CPHI,SPHI and Height.
 * Iteration of CPHI and SPHI to 10**-12 radian resp.
 * 2*10**-7 arcsec.
 * --------------------------------------------------------------
 */
    CT = Z/RR;
    ST = P/RR;
    {
        const double denominator = 1.0-gi->Geocent_e2*(2.0-gi->Geocent_e2)*ST*ST;
        if( denominator == 0 )
        {
            *Latitude = HUGE_VAL;
            *Longitude = HUGE_VAL;
            *Height = HUGE_VAL;
            return;
        }
        RX = 1.0/sqrt(denominator);
    }
    CPHI0 = ST*(1.0-gi->Geocent_e2)*RX;
    SPHI0 = CT*RX;
    iter = 0;

/* loop to find sin(Latitude) resp. Latitude
 * until |sin(Latitude(iter)-Latitude(iter-1))| < genau */
    do
    {
        iter++;
        RN = gi->Geocent_a/sqrt(1.0-gi->Geocent_e2*SPHI0*SPHI0);

/*  ellipsoidal (geodetic) height */
        *Height = P*CPHI0+Z*SPHI0-RN*(1.0-gi->Geocent_e2*SPHI0*SPHI0);

        /* avoid zero division */
        if (RN+*Height==0.0) {
            *Latitude = 0.0;
            return;
        }
        RK = gi->Geocent_e2*RN/(RN+*Height);
        {
            const double denominator = 1.0-RK*(2.0-RK)*ST*ST;
            if( denominator == 0 )
            {
                *Latitude = HUGE_VAL;
                *Longitude = HUGE_VAL;
                *Height = HUGE_VAL;
                return;
            }
            RX = 1.0/sqrt(denominator);
        }
        CPHI = ST*(1.0-RK)*RX;
        SPHI = CT*RX;
        SDPHI = SPHI*CPHI0-CPHI*SPHI0;
        CPHI0 = CPHI;
        SPHI0 = SPHI;
    }
    while (SDPHI*SDPHI > genau2 && iter < maxiter);

/*	ellipsoidal (geodetic) latitude */
    *Latitude=atan2(SPHI, fabs(CPHI));

#endif /* defined(USE_ITERATIVE_METHOD) */
} /* END OF Convert_Geocentric_To_Geodetic */



GeoPoint::GeoPoint(double const _x, double const _y, double const _z) :
  m_x(_x), m_y(_y), m_z(_z) {
}

GeoPoint::GeoPoint(QGeoCoordinate const c) {
  // construct a geocetric point (x,y,z) from a Geodetic Coordinate
  // (latitude, longitude, altitude).
  pj_Convert_Geodetic_To_Geocentric(&pj_wgs84, c.latitude(), c.longitude(), c.altitude(), &m_x, &m_y, &m_z);
}

double GeoPoint::x() const {
  return m_x;
}

double GeoPoint::y() const {
  return m_y;
}

double GeoPoint::z() const {
  return m_z;
}

void GeoPoint::setX(double x){
  m_x = x;
}

void GeoPoint::setY(double y){
  m_y = y;
}

void GeoPoint::setZ(double z){
  m_z = z;
}

void GeoPoint::set(double const _x, double const _y, double const _z) {
  m_x = _x; m_y = _y; m_z = _z;
}

void GeoPoint::set(QGeoCoordinate const c) {
  pj_Convert_Geodetic_To_Geocentric(&pj_wgs84, c.latitude(), c.longitude(), c.altitude(), &m_x, &m_y, &m_z);
}

double GeoPoint::distanceTo (GeoPoint const &to) const {
  const double dx = this->x() - to.x();
  const double dy = this->y() - to.y();
  const double dz = this->z() - to.z();
  return qSqrt (dx*dx + dy*dy + dz*dz);
}

QGeoCoordinate GeoPoint::coordinate() const {
  double latitude, longitude, altitude;
  pj_Convert_Geocentric_To_Geodetic(&pj_wgs84, m_x, m_y, m_z, &latitude, &longitude, &altitude);
  return QGeoCoordinate(latitude, longitude, altitude);
}
