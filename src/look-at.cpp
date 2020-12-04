// Code was ported to C from http://cosinekitty.com/compass.html

#include "look-at.hpp"
#include "vector3d.hpp"
#include <QtMath>

static
double EarthRadiusInMeters (const double latitudeRadians)
{
  // latitude is geodetic, i.e. that reported by GPS
  // http://en.wikipedia.org/wiki/Earth_radius
  const double a = 6378137.0;  // equatorial radius in meters
  const double b = 6356752.3;  // polar radius in meters
  const double cos_ = qCos (latitudeRadians);
  const double sin_ = qSin (latitudeRadians);
  const double t1 = a * a * cos_;
  const double t2 = b * b * sin_;
  const double t3 = a * cos_;
  const double t4 = b * sin_;
  return qSqrt ((t1*t1 + t2*t2) / (t3*t3 + t4*t4));
}
    
static
double GeocentricLatitude(const double lat)
{
  // Convert geodetic latitude 'lat' to a geocentric latitude 'clat'.
  // Geodetic latitude is the latitude as given by GPS.
  // Geocentric latitude is the angle measured from center of Earth between a point and the equator.
  // https://en.wikipedia.org/wiki/Latitude#Geocentric_latitude
  const double e2 = 0.00669437999014;
  const double clat = qAtan((1.0 - e2) * tan(lat));
  return clat;
}

static
void ConvertLocationToPoint(QGeoCoordinate const &c, Vector3D *rval, Vector3D *normal, double *radius_at_point)
{
  // Convert (lat, lon, elv) to (x, y, z). Also return the normal and radius of the geoid at the point.
  const double lat = c.latitude() * M_PI / 180.0;
  const double lon = c.longitude() * M_PI / 180.0;
  const double radius = EarthRadiusInMeters(lat);
  const double clat   = GeocentricLatitude(lat);
        
  const double cosLon = qCos(lon);
  const double sinLon = qSin(lon);
  const double cosLat = qCos(clat);
  const double sinLat = qSin(clat);
  double x = radius * cosLon * cosLat;
  double y = radius * sinLon * cosLat;
  double z = radius * sinLat;
        
  // We used geocentric latitude to calculate (x,y,z) on the Earth's ellipsoid.
  // Now we use geodetic latitude to calculate normal vector from the surface, to correct for elevation.
  const double cosGlat = qCos(lat);
  const double sinGlat = qSin(lat);
  
  const double nx = cosGlat * cosLon;
  const double ny = cosGlat * sinLon;
  const double nz = sinGlat;
  
  x += c.altitude() * nx;
  y += c.altitude() * ny;
  z += c.altitude() * nz;

  if (rval)
    rval->set(x, y, z);

  // return the normal to the point
  if (normal)
    normal->set(nx, ny, nz);
  
  // return the radius of the geoid at the point
  if (radius_at_point)
    *radius_at_point = radius;
}

static
Vector3D RotateGlobe(QGeoCoordinate const &b, QGeoCoordinate const &a)
{
  // Get modified coordinates of 'b' by rotating the globe so that 'a' is at lat=0, lon=0.
  double cos_a, sin_a;
  double alat;
  QGeoCoordinate br (b.latitude(), b.longitude() - a.longitude(), b.altitude());
  Vector3D brp;

  ConvertLocationToPoint(br, &brp, 0, 0);

  // Rotate brp cartesian coordinates around the z-axis by a.lon degrees,
  // then around the y-axis by a.lat degrees.
  // Though we are decreasing by a.lat degrees, as seen above the y-axis,
  // this is a positive (counterclockwise) rotation (if B's longitude is east of A's).
  // However, from this point of view the x-axis is pointing left.
  // So we will look the other way making the x-axis pointing right, the z-axis
  // pointing up, and the rotation treated as negative.

  alat = GeocentricLatitude(-a.latitude() * M_PI / 180.0);
  cos_a = qCos(alat);
  sin_a = qSin(alat);

  return Vector3D((brp.x() * cos_a) - (brp.z() * sin_a),
                  brp.y(),
                  (brp.x() * sin_a) + (brp.z() * cos_a));
}

static
Vector3D *NormalizeVectorDiff(Vector3D const &b, Vector3D const &a, Vector3D *rval)
{
  // Calculate norm(b-a), where norm divides a vector by its length to produce a unit vector.
  // returns NULL upon error.
  const double dx = b.x() - a.x();
  const double dy = b.y() - a.y();
  const double dz = b.z() - a.z();
  const double dist2 = dx*dx + dy*dy + dz*dz;
  double dist;
  if (dist2 == 0) {
    return 0L;
  }
  dist = qSqrt(dist2);
  if (rval)
    rval->set(dx/dist, dy/dist, dz/dist);
  return rval;
}

void lookAt(QGeoCoordinate const &a, QGeoCoordinate const &b, double *los_distance, double *azimuth, double *elevation)
{
  Vector3D ap, bp, br, bma;
  Vector3D ap_normal;
  
  ConvertLocationToPoint(a, &ap, &ap_normal, 0);
  ConvertLocationToPoint(b, &bp, 0, 0);
  *los_distance = ap.distanceTo(bp);

  // Let's use a trick to calculate azimuth:
  // Rotate the globe so that point A looks like latitude 0, longitude 0.
  // We keep the actual radii calculated based on the oblate geoid,
  // but use angles based on subtraction.
  // Point A will be at x=radius, y=0, z=0.
  // Vector difference B-A will have dz = N/S component, dy = E/W component.                
  br = RotateGlobe (b, a);
  if ((br.z()*br.z() + br.y()*br.y()) > 1.0e-6) {
    double theta = qAtan2(br.z(), br.y()) * 180.0 / M_PI;
    double _azimuth = 90.0 - theta;
    if (_azimuth < 0.0) {
      _azimuth += 360.0;
    }
    if (_azimuth > 360.0) {
      _azimuth -= 360.0;
    }
    *azimuth = _azimuth;
  }
                
  if (NormalizeVectorDiff(bp, ap, &bma)) {
    // Calculate elevation, which is the angle above the horizon of B as seen from A.
    // Almost always, B will actually be below the horizon, so the altitude will be negative.
    // The dot product of bma and norm = cos(zenith_angle), and zenith_angle = (90 deg) - altitude.
    // So altitude = 90 - acos(dotprod).
    *elevation = 90.0 - (180.0 / M_PI)*
      qAcos(bma.x()*ap_normal.x() + bma.y()*ap_normal.y() + bma.z()*ap_normal.z());
  }
}
