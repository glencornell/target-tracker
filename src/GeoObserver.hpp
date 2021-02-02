#pragma once

#include <QObject>
#include "LookAngle.hpp"
#include "GeoEntity.hpp"
#include "GeoTarget.hpp"

// A GeoObserver is a type of GeoEntity that can point at another
// object in space.  A gimballed camera is an example of a
// GeoObserver.  Therefore, a GeoObserver not only has a position and
// orientation, but also allows one to change it's orientation to
// "look at" a point in space, a heading or another GeoEntity.

class GeoObserver : public GeoEntity
{
  Q_OBJECT
  Q_PROPERTY(GeoTarget *target    READ target    WRITE setTarget NOTIFY targetChanged)
  Q_PROPERTY(LookAngle  lookAngle READ lookAngle                 NOTIFY lookAngleChanged)
public:
  GeoObserver(QObject *parent = nullptr);
  GeoObserver(QUuid const &uuid);
  ~GeoObserver();

  GeoTarget *target() const;
  LookAngle lookAngle() const;

signals:
  void targetChanged();
  void lookAngleChanged(LookAngle const &lookAngle);

public slots:
  void setTarget(GeoTarget *target);
  void onObserverPositionChanged(QGeoPositionInfo const &position);
  void onTargetPositionChanged(QGeoPositionInfo const &position);

private:
  void setLookAngle();
  
  // What are we looking at?
  GeoTarget *m_target;

  // What are the az/el from the GeoEntity's vantage (observer) to the target?
  LookAngle  m_lookAngle;
};
