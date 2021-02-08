#pragma once

#include <QObject>
#include <QCoreApplication>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include "GeoEntity.hpp"
#include "GeoObserver.hpp"
#include "LookAngle.hpp"

class BlosApp : public QObject
{
  Q_OBJECT

public:
  BlosApp(QCoreApplication *app, int argc, char *argv[]);

public slots:
  void main();

private slots:
  void onObserverPositionChanged(QGeoPositionInfo const &info);
  void onObservedPositionChanged(QGeoPositionInfo const &info);
  void onLookAngleChanged(LookAngle const &info);
  void onPositionChanged(QGeoPositionInfo const &info);
  void onError(QGeoPositionInfoSource::Error error);

signals:
  void finished();

private:
  QCoreApplication       *m_app;
  GeoEntity              *observer;
  GeoObserver            *gimbal;
  GeoEntity              *observed;
  QGeoPositionInfoSource *source;
};

