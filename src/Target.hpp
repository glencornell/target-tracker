#pragma once

#include <QtCore>
#include <QGeoCoordinate>
#include "Direction.hpp"
#include "Asset.hpp"

// A target is something we can look at.  It can be nothing, a
// geographic coordinate, an asset, or a direction.
class Target
{
  Q_GADGET
  Q_PROPERTY(TargetType targetType READ targetType)
public:
  // The discriminant: what we're looking at.
  enum TargetType {
    TARGET_NONE,      // Look at nothing
    TARGET_DIRECTION, // Look in a compass direction and elevation above the horizon
    TARGET_POSITION,  // Look at a geographic coordinate (and altitude)
    TARGET_ASSET      // Look at an asset
  };
  Q_ENUM(TargetType);

public:
  Target();
  Target(QGeoCoordinate position);
  Target(Asset *asset);
  Target(Direction direction);
  Target(Target const &other);
  ~Target();
  Target &operator=(Target const &other);
  bool operator==(const Target &other) const;
  inline bool operator!=(Target const &other) {
    return !operator==(other);
  }

  TargetType targetType() const;
  Direction direction() const;
  QGeoCoordinate position() const;
  Asset *asset() const;

private:
  // targets (there may be only one at a time, based upon the value of
  // the discriminant):
  TargetType        m_targetType; // What are we looking at?
  union {
    Direction       m_direction;
    QGeoCoordinate  m_position;
    Asset          *m_asset;
  };
};

