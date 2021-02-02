#pragma once

#include <QtMath>
#include <QGeoCoordinate>

// A GeoPoint is a geocentric point in an Earth-Centered, Earth Fixed
// Cartesian coordinate system.  This class is similar to QVector3D
// but uses double precision floats instead of single precision.
// GeoPoint is derived from Q_GADGET as opposed to from Q_OBJECT so
// that it may be assigned (QObject is copy protected) and it may be
// used in QVariant (for automatic inclusion in XML, JSON, and other
// serialized streams).
class GeoPoint {
  Q_GADGET
  
  Q_PROPERTY(double x READ x WRITE setX)
  Q_PROPERTY(double y READ y WRITE setY)
  Q_PROPERTY(double z READ z WRITE setZ)

public:
  GeoPoint() { }
  GeoPoint(double const _x, double const _y, double const _z);
  GeoPoint(QGeoCoordinate const c);
  double x() const;
  double y() const;
  double z() const;
  void setX(double x);
  void setY(double y);
  void setZ(double z);
  void set(double const _x, double const _y, double const _z);
  void set(QGeoCoordinate const c);

  // Calculate the Euclidian (aka "line of sight") distance to the
  // given point.
  double distanceTo (GeoPoint const &to) const;

  // Convert to a Geodetic coordinate:
  QGeoCoordinate convert() const;
  
private:
  double m_x;
  double m_y;
  double m_z;
};

// Make GeoPoint known to QMetaType so that it may be used as a custom
// type in QVariant.
Q_DECLARE_METATYPE(GeoPoint)
