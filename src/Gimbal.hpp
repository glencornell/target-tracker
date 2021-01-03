#pragma once

#include <QObject>
#include "Direction.hpp"
#include "Target.hpp"
#include "Asset.hpp"

// A Gimbal is a type of asset that can point at another object in
// space.  Therefore, a Gimbal not only has a position and orientation,
// but also allows one to change it's orientation to "look at" a point
// in space, a heading or another asset.

class Gimbal : public Asset
{
  Q_OBJECT
  Q_PROPERTY(Target *target READ target WRITE setTarget NOTIFY targetChanged)
  Q_PROPERTY(Direction lookAngle READ lookAngle NOTIFY lookAngleChanged)
public:
  Gimbal(Asset *parent = nullptr);
  Gimbal(QUuid const &uuid);
  ~Gimbal();

  void setParent(Asset *parent);
  Target *target() const;
  Direction lookAngle() const;

signals:
  void targetChanged();
  void lookAngleChanged(Direction const &direction);

public slots:
  void setTarget(Target *target);
  void onObserverPositionChanged(QGeoPositionInfo const &position);
  void onTargetPositionChanged(QGeoPositionInfo const &position);

private:
  void setLookAngle();
  
  // What are we attached to?
  Asset        *m_parent;
  
  // What are we looking at?
  Target       *m_target;

  // What are the az/el from the Asset's vantage (observer) to the target?
  Direction     m_lookAngle;
};
