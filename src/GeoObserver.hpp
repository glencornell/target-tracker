#pragma once

#include <QObject>
#include <QGeoCoordinate>
#include "GeoEntity.hpp"
#include "LookAngle.hpp"

// A GeoObserver is a type of GeoEntity that can point at another
// object in space.  A gimballed camera or a radio telescope are
// examples of GeoObservers.  A GeoObserver not only has a position
// and orientation, but also allows one to change it's orientation to
// "look at" a point in space, a heading or another GeoEntity.

// The calculated look angle is in the Earth cetered, earth fixed
// reference frame. In other words, the azimuth is measured from true
// North and the elevation is from the horizon, regardless of the
// observer's position and orientation.

class GeoObserver : public GeoEntity
{
  Q_OBJECT
  Q_PROPERTY(LookAngle  lookAngle READ lookAngle                 NOTIFY lookAngleChanged)
public:
  // The discriminant: what we're looking at.  sometimes called the
  // pointing mode.
  enum TargetType {
    TARGET_NONE,       // Look at nothing
    TARGET_LOOK_ANGLE, // Look in a direction (azimuth and elevation)
    TARGET_COORDINATE, // Look at a geographic coordinate (and altitude)
    TARGET_ENTITY      // Look at an entity
  };
  Q_ENUM(TargetType);

  GeoObserver(QObject *parent = nullptr);
  GeoObserver(QUuid const &uuid);
  ~GeoObserver();

  LookAngle lookAngle() const;

  // Setting the pointing mode:
  void setTarget();                                      // look at nothing
  void setTarget(QGeoCoordinate const position);         // look at a fixed position
  void setTarget(LookAngle const commanded_lookAngle);   // look in a fixed direction
  void setTarget(GeoEntity *entity);                     // look at a an entity (that might be moving)

signals:
  void targetChanged(TargetType &targetType);
  void lookAngleChanged(LookAngle const &lookAngle);

public slots:
  virtual void onTargetPositionChanged(QGeoPositionInfo const &position);
  void positionChanged(QGeoPositionInfo const &position);

protected:
  void calculateLookAngle();

private:
  // The discriminant of the anonymous union
  TargetType        m_targetType; // What are we looking at?

  // We can look at only one thing at a time.  Therefore, this is a
  // union discriminated by m_targetType
  union {
    LookAngle       m_commanded_lookAngle;
    QGeoCoordinate  m_coordinate;
    GeoEntity      *m_entity;
  };
  
  // The az/el from the GeoEntity's vantage (observer) to the target.
  // This is not to be confused with the commanded look angle above.
  // Rather, this is the calculated look angle to be sent to the
  // hardware pointing device.
  LookAngle  m_lookAngle;
};
