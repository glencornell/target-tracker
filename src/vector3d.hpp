#pragma once

#include <QtMath>

// A point in 3D space in a cartesian coordinate system.  Similar to
// QVector3D but uses double precision floats instead of single
// precision.
class Vector3D {
private:
  double m_x;
  double m_y;
  double m_z;
public:
  Vector3D() { }
  Vector3D(double _x, double _y, double _z) :
    m_x(_x), m_y(_y), m_z(_z) { }
  double x() const { return m_x; }
  double y() const { return m_y; }
  double z() const { return m_z; }
  void set(double _x, double _y, double _z) {
    m_x = _x;
    m_y = _y;
    m_z = _z;
  }
  
  double distanceTo (Vector3D const &to) const {
    const double dx = this->x() - to.x();
    const double dy = this->y() - to.y();
    const double dz = this->z() - to.z();
    return qSqrt (dx*dx + dy*dy + dz*dz);
  }

};

