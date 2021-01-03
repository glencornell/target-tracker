#pragma once

#include <QObject>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include "Asset.hpp"
#include "Gimbal.hpp"
#include "Direction.hpp"

class GeoObserver : public QObject
{
  Q_OBJECT

public:
  GeoObserver(QObject * parent = nullptr);

private slots:
  void main();
  void onObserverPositionChanged(QGeoPositionInfo const &info);
  void onObservedPositionChanged(QGeoPositionInfo const &info);
  void onLookAngleChanged(Direction const &info);
  void onPositionChanged(QGeoPositionInfo const &info);
  void onError(QGeoPositionInfoSource::Error error);

signals:
  void finished();

private:
  Asset  *observer;
  Gimbal *gimbal;
  Asset  *observed;
  QGeoPositionInfoSource *source;
};

