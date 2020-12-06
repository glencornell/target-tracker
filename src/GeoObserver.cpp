#include <QTextStream>
#include <QDateTime>
#include "GeoObserver.hpp"
#include "LogFilePositionSource.hpp"
#include "look-at.hpp"

GeoObserver::GeoObserver(QObject * /* parent */)
{
  observer = new Asset(this);
  observed = new Asset(this);

  // Statically set the observer's position:
  observer->setPosition(QGeoPositionInfo (QGeoCoordinate (34.24333333, 118.0975000, 1877.873), QDateTime::currentDateTimeUtc()));

  // The observed position will be obtained from the logfile position
  // info source:
  LogFilePositionSource *observed_source = new LogFilePositionSource(this);
  source = observed_source;

  // Connect the position info source to the observed object so that
  // it moves over time.
  connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)), observed, SLOT(onPositionChanged(QGeoPositionInfo)));

  // Also connect it to calculate the new look angle to the observer's position
  connect(observed, SIGNAL(positionChanged(QGeoPositionInfo)), this, SLOT(onPositionChanged(QGeoPositionInfo)));

  // Connect the position info source's error signal to terminate the
  // program:
  connect(source, SIGNAL(error(QGeoPositionInfoSource::Error)), this, SLOT(onError(QGeoPositionInfoSource::Error)));
}

void GeoObserver::onPositionChanged(QGeoPositionInfo const &info)
{
  QTextStream stream(stdout);
  double distance;
  double azimuth;
  double elevation;
  
  lookAt(observer->position().coordinate(), info.coordinate(), &distance, &azimuth, &elevation);

  stream << "======================" << Qt::endl;
  stream << " observer's location: " << observer->position().coordinate().toString() << Qt::endl;
  stream << " observed's location: " << info.coordinate().toString() << Qt::endl;
  stream << " distance to point b: " << distance << Qt::endl;
  stream << "  azimuth to point b: " << azimuth << Qt::endl;
  stream << "elevation to point b: " << elevation << Qt::endl;
}

void GeoObserver::main()
{
  // Tell the observed object to start moving along its path:
  source->startUpdates();
}

void GeoObserver::onError(QGeoPositionInfoSource::Error)
{
  // Tell the observed object to stop reporting updates:
  source->stopUpdates();
  
  // clean up:
  delete observer;
  delete observed;
  delete source;
  
  // signal that we're done:
  emit finished();
}
