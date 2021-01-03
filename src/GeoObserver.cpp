#include <QTextStream>
#include <QDateTime>
#include "GeoObserver.hpp"
#include "data-sources/LogFilePositionSource.hpp"
#include "look-at.hpp"

GeoObserver::GeoObserver(QObject * /* parent */)
{
  observer = new Asset();
  gimbal   = new Gimbal(observer);
  observed = new Asset();

  // print the object's movements
  connect(observer, &Asset::positionChanged,   this, &GeoObserver::onObserverPositionChanged);
  connect(observed, &Asset::positionChanged,   this, &GeoObserver::onObservedPositionChanged);
  connect(gimbal,   &Gimbal::lookAngleChanged, this, &GeoObserver::onLookAngleChanged);

  // Statically set the observer's position:
  observer->setPosition(QGeoPositionInfo (QGeoCoordinate (34.24333333, 118.0975000, 1877.873), QDateTime::currentDateTimeUtc()));

  // The observed position will be obtained from the logfile position
  // info source:
  LogFilePositionSource *observed_source = new LogFilePositionSource(this);
  observed->setPositionSource(observed_source);
  source = observed_source;

  // Point the gimbal at the observed target
  Target *target = new Target(observed);
  gimbal->setTarget(target);
  
  // Connect the position info source's error signal to terminate the
  // program:
  connect(observed_source, qOverload<QGeoPositionInfoSource::Error>(&LogFilePositionSource::error), this, &GeoObserver::onError);
}

void GeoObserver::onObserverPositionChanged(QGeoPositionInfo const &position)
{
  QTextStream stream(stdout);
  stream << " observer's location: " << position.coordinate().toString() << Qt::endl;
}

void GeoObserver::onObservedPositionChanged(QGeoPositionInfo const &position)
{
  QTextStream stream(stdout);
  stream << " observed's location: " << position.coordinate().toString() << Qt::endl;
}

void GeoObserver::onLookAngleChanged(Direction const &lookAngle)
{
  QTextStream stream(stdout);
  stream << "  azimuth to observed: " << lookAngle.azimuth() << Qt::endl;
  stream << "elevation to observed: " << lookAngle.elevation() << Qt::endl;
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
  // Tell the objects to start moving:
  observer->startUpdates();
  observed->startUpdates();
}

void GeoObserver::onError(QGeoPositionInfoSource::Error error)
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
