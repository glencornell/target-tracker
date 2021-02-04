#include <QTextStream>
#include <QDateTime>
#include "BlosApp.hpp"
#include "data-sources/LogFilePositionSource.hpp"
#include "GeoPoint.hpp"

BlosApp::BlosApp(QObject * /* parent */)
{
  observer = new GeoEntity();
  gimbal   = new GeoObserver();
  observed = new GeoEntity();

  // The gimbal's position is tied to the position of the observer
  connect(observer, &GeoEntity::positionChanged, gimbal, &GeoObserver::positionChanged);

  // The observed position updates will be obtained from the logfile
  // position info source:
  LogFilePositionSource *observed_source = new LogFilePositionSource(this);
  connect(observed_source, &LogFilePositionSource::positionUpdated, observed, &GeoEntity::setPosition);
  source = observed_source;

  // Point the gimbal at the observed target
  gimbal->setTarget(observed);

  // Print everyone's movements
  connect(observer, &GeoEntity::positionChanged,    this, &BlosApp::onObserverPositionChanged);
  connect(observed, &GeoEntity::positionChanged,    this, &BlosApp::onObservedPositionChanged);
  connect(gimbal,   &GeoObserver::lookAngleChanged, this, &BlosApp::onLookAngleChanged);

  // Connect the position info source's error signal to terminate the
  // program:
  connect(observed_source, qOverload<QGeoPositionInfoSource::Error>(&LogFilePositionSource::error), this, &BlosApp::onError);
  
  // The observer doesn't move, so we can statically set the
  // observer's position.  Set the position after the connections have
  // been made so that the objects get the initial conditions:
  observer->setPosition(QGeoPositionInfo (QGeoCoordinate (34.24333333, 118.0975000, 1877.873), QDateTime::currentDateTimeUtc()));
}

void BlosApp::onObserverPositionChanged(QGeoPositionInfo const &position)
{
  QTextStream stream(stdout);
  stream << " observer's location: " << position.coordinate().toString() << Qt::endl;
}

void BlosApp::onObservedPositionChanged(QGeoPositionInfo const &position)
{
  QTextStream stream(stdout);
  stream << "   target's location: " << position.coordinate().toString() << Qt::endl;
}

void BlosApp::onLookAngleChanged(LookAngle const &lookAngle)
{
  QTextStream stream(stdout);
  stream << "  azimuth to target: " << lookAngle.azimuth() << Qt::endl;
  stream << "elevation to target: " << lookAngle.elevation() << Qt::endl;
}

void BlosApp::onPositionChanged(QGeoPositionInfo const &info)
{
  QTextStream stream(stdout);
  LookAngle lookAngle(observer->position().coordinate(), info.coordinate());

  stream << "========================" << Qt::endl;
  stream << "   observer's location: " << observer->position().coordinate().toString() << Qt::endl;
  stream << "   observed's location: " << info.coordinate().toString() << Qt::endl;
  stream << "     azimuth to target: " << lookAngle.azimuth() << Qt::endl;
  stream << "   elevation to target: " << lookAngle.elevation() << Qt::endl;
  stream << "LoS distance to target: " << GeoPoint(observer->position().coordinate()).distanceTo(GeoPoint(info.coordinate())) << Qt::endl;
}

void BlosApp::main()
{
  // Tell the position sources to start reporting updates:
  source->startUpdates();
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
