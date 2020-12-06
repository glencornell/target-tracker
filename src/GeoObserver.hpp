#pragma once

#include <QObject>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include "Asset.hpp"

class GeoObserver : public QObject
{
  Q_OBJECT

public:
  GeoObserver(QObject * parent = nullptr);

private slots:
  void main();
  void onPositionChanged(QGeoPositionInfo const &info);
  void onError(QGeoPositionInfoSource::Error);

signals:
  void finished();

private:
  Asset *observer;
  Asset *observed;
  QGeoPositionInfoSource *source;
};

