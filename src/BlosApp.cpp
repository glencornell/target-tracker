#include <QTextStream>
#include <QDateTime>
#include "BlosApp.hpp"
#include "data-sources/LogFilePositionSource.hpp"
#include "look-at.hpp"

BlosApp::BlosApp(QObject * /* parent */)
{
  observer = new GeoEntity();
  gimbal   = new GeoObserver(observer);
  observed = new GeoEntity();

  // print the object's movements
  connect(observer, &GeoEntity::positionChanged,    this, &BlosApp::onObserverPositionChanged);
  connect(observed, &GeoEntity::positionChanged,    this, &BlosApp::onObservedPositionChanged);
  connect(gimbal,   &GeoObserver::lookAngleChanged, this, &BlosApp::onLookAngleChanged);

  // Statically set the observer's position:
  observer->setPosition(QGeoPositionInfo (QGeoCoordinate (34.24333333, 118.0975000, 1877.873), QDateTime::currentDateTimeUtc()));

  // The observed position will be obtained from the logfile position
  // info source:
  LogFilePositionSource *observed_source = new LogFilePositionSource(this);
  observed->setPositionSource(observed_source);
  source = observed_source;

  // Point the gimbal at the observed target
  GeoTarget *target = new GeoTarget(observed);
  gimbal->setTarget(target);
  
  // Connect the position info source's error signal to terminate the
  // program:
  connect(observed_source, qOverload<QGeoPositionInfoSource::Error>(&LogFilePositionSource::error), this, &BlosApp::onError);
}

void BlosApp::onObserverPositionChanged(QGeoPositionInfo const &position)
{
  QTextStream stream(stdout);
  stream << " observer's location: " << position.coordinate().toString() << Qt::endl;
}

void BlosApp::onObservedPositionChanged(QGeoPositionInfo const &position)
{
  QTextStream stream(stdout);
  stream << " observed's location: " << position.coordinate().toString() << Qt::endl;
}

void BlosApp::onLookAngleChanged(Direction const &lookAngle)
{
  QTextStream stream(stdout);
  stream << "  azimuth to observed: " << lookAngle.azimuth() << Qt::endl;
  stream << "elevation to observed: " << lookAngle.elevation() << Qt::endl;
}

void BlosApp::onPositionChanged(QGeoPositionInfo const &info)
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

void BlosApp::main()
{
  // Tell the objects to start moving:
  observer->startUpdates();
  observed->startUpdates();
}

void BlosApp::onError(QGeoPositionInfoSource::Error error)
{
  Q_UNUSED(error)
    
  // Tell the observed object to stop reporting updates:
  source->stopUpdates();
  
  // clean up:
  delete observer;
  delete observed;
  delete source;
  
  // signal that we're done:
  emit finished();
}
