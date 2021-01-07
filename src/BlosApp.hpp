#pragma once

#include <QObject>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include "GeoEntity.hpp"
#include "GeoObserver.hpp"
#include "Direction.hpp"

class BlosApp : public QObject
{
  Q_OBJECT

public:
  BlosApp(QObject * parent = nullptr);

public slots:
  void main();

private slots:
  void onObserverPositionChanged(QGeoPositionInfo const &info);
  void onObservedPositionChanged(QGeoPositionInfo const &info);
  void onLookAngleChanged(Direction const &info);
  void onPositionChanged(QGeoPositionInfo const &info);
  void onError(QGeoPositionInfoSource::Error error);

signals:
  void finished();

private:
  GeoEntity              *observer;
  GeoObserver            *gimbal;
  GeoEntity              *observed;
  QGeoPositionInfoSource *source;
};

