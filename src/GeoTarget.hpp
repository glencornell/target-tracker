#pragma once

#include <QtCore>
#include <QGeoCoordinate>
#include "LookAngle.hpp"
#include "GeoEntity.hpp"

// A GeoTarget is something we can look at.  It can be nothing, a
// geographic coordinate, a GeoEntity, or a look angle (direction).
class GeoTarget
{
  Q_GADGET
  Q_PROPERTY(TargetType      targetType READ targetType)
  Q_PROPERTY(LookAngle       lookAngle  READ lookAngle  WRITE setLookAngle)
  Q_PROPERTY(QGeoCoordinate  coordinate READ coordinate WRITE setCoordinate)
  Q_PROPERTY(GeoEntity      *entity     READ entity     WRITE setEntity)
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

public:
  GeoTarget();
  GeoTarget(QGeoCoordinate coordinate);
  GeoTarget(GeoEntity *entity);
  GeoTarget(LookAngle lookAngle);
  GeoTarget(GeoTarget const &other);
  ~GeoTarget();
  GeoTarget &operator=(GeoTarget const &other);
  bool operator==(const GeoTarget &other) const;
  inline bool operator!=(GeoTarget const &other) {
    return !operator==(other);
  }

  // accessors
  TargetType targetType() const;
  LookAngle lookAngle() const;
  void setLookAngle(LookAngle lookAngle);
  QGeoCoordinate coordinate() const;
  void setCoordinate(QGeoCoordinate coordinate);
  GeoEntity *entity() const;
  void setEntity(GeoEntity *entity);

  // What type of GeoTarget is this?
  bool isNothing() const;
  bool isLookAngle() const;
  bool isCoordinate() const;
  bool isEntity() const;

private:
  // The discriminant of the anonymous union
  TargetType        m_targetType; // What are we looking at?

  // We can look at only one thing at a time.
  union {
    LookAngle       m_lookAngle;
    QGeoCoordinate  m_coordinate;
    GeoEntity      *m_entity;
  };
};

