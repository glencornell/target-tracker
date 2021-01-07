#pragma once

#include <QtCore>
#include <QGeoCoordinate>
#include "Direction.hpp"
#include "GeoEntity.hpp"

// A GeoTarget is something we can look at.  It can be nothing, a
// geographic coordinate, a GeoEntity, or a direction.
class GeoTarget
{
  Q_GADGET
  Q_PROPERTY(TargetType      targetType READ targetType)
  Q_PROPERTY(Direction       direction  READ direction  WRITE setDirection)
  Q_PROPERTY(QGeoCoordinate  coordinate READ coordinate WRITE setCoordinate)
  Q_PROPERTY(GeoEntity      *entity     READ entity     WRITE setEntity)
public:
  // The discriminant: what we're looking at.
  enum TargetType {
    TARGET_NONE,       // Look at nothing
    TARGET_DIRECTION,  // Look in a compass direction and elevation above the horizon
    TARGET_COORDINATE, // Look at a geographic coordinate (and altitude)
    TARGET_ENTITY      // Look at an entity
  };
  Q_ENUM(TargetType);

public:
  GeoTarget();
  GeoTarget(QGeoCoordinate coordinate);
  GeoTarget(GeoEntity *entity);
  GeoTarget(Direction direction);
  GeoTarget(GeoTarget const &other);
  ~GeoTarget();
  GeoTarget &operator=(GeoTarget const &other);
  bool operator==(const GeoTarget &other) const;
  inline bool operator!=(GeoTarget const &other) {
    return !operator==(other);
  }

  // accessors
  TargetType targetType() const;
  Direction direction() const;
  void setDirection(Direction direction);
  QGeoCoordinate coordinate() const;
  void setCoordinate(QGeoCoordinate coordinate);
  GeoEntity *entity() const;
  void setEntity(GeoEntity *entity);

  // What type of GeoTarget is this?
  bool isNothing() const;
  bool isDirection() const;
  bool isCoordinate() const;
  bool isEntity() const;

private:
  // The discriminant of the anonymous union
  TargetType        m_targetType; // What are we looking at?

  // This can only be looing at one thing at a time.
  union {
    Direction       m_direction;
    QGeoCoordinate  m_coordinate;
    GeoEntity      *m_entity;
  };
};

