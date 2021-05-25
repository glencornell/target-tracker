#pragma once

#include <QObject>
#include <QCoreApplication>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include "GeoEntity.hpp"
#include "GeoObserver.hpp"
#include "LookAngle.hpp"

class TargetTrackerApp : public QObject
{
  Q_OBJECT

public:
  TargetTrackerApp(QCoreApplication *app, int argc, char *argv[]);

public slots:
  void main();

private slots:
  void onObserverPositionChanged(QGeoPositionInfo const &info);
  void onTargetPositionChanged(QGeoPositionInfo const &info);
  void onLookAngleChanged(LookAngle const &info);
  void onPositionChanged(QGeoPositionInfo const &info);
  void onError(QGeoPositionInfoSource::Error error);

signals:
  void finished();

private:
  void create_observer();
  void create_target();
  
private:
  QCoreApplication       *m_app;
  
  GeoObserver            *observer;
  QGeoPositionInfoSource *observer_source;
  
  GeoEntity              *target;
  QGeoPositionInfoSource *target_source;
};

