#pragma once

#include <QtCore/QMetaType>
#include <QGeoCoordinate>

// The Look angle is the direction in which the observer must gaze in
// order to see the target. The look angle is represented in the
// azimuth angle in degrees from true north and the elevation in
// degrees above the horizon (the tangential plane which is normal to
// the local gravity vector). A look angle is a term used from
// astronomy that is used to describe how to point a telescope at a
// satellite.
//
// This type is derived from Q_GADGET as opposed to from Q_OBJECT so
// that it may be assigned (QObject is copy protected) and so that it
// may be used in QVariant (for automatic inclusion in XML, JSON, and
// other serialized streams).

class LookAngle
{
  Q_GADGET
  
  Q_PROPERTY(float azimuth READ azimuth WRITE setAzimuth)
  Q_PROPERTY(float elevation READ elevation WRITE setElevation)
  
public:
  
  LookAngle();
  LookAngle(float azimuth, float elevation);
  LookAngle(QGeoCoordinate const &observer, QGeoCoordinate const &target);
  LookAngle(LookAngle const &other);
  ~LookAngle();

  LookAngle &operator=(LookAngle const &other);
  bool operator==(const LookAngle &other) const;
  bool operator!=(const LookAngle &other) const;
  LookAngle &operator+=(const LookAngle &other);
  LookAngle &operator-=(const LookAngle &other);
  
  float azimuth() const;
  void setAzimuth(float azimuth);
  
  float elevation() const;
  void setElevation(float elevation);

  // calculate the look angle of the target from the observer's
  // reference.
  void setLookAngle(QGeoCoordinate const &observer, QGeoCoordinate const &target);
  
private:
  friend bool qFuzzyCompare(LookAngle const &p1, LookAngle const &p2);
  
  float m_azimuth;   // degrees from true north
  float m_elevation; // degrees from the horizon
};

// Make LookAngle known to QMetaType so that it may be used as a
// custom type in QVariant.
Q_DECLARE_METATYPE(LookAngle)

bool qFuzzyCompare(LookAngle const &p1, LookAngle const &p2);
