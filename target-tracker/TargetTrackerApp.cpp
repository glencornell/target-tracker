#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include "TargetTrackerApp.hpp"
#include "data-sources/LogFilePositionSource.hpp"
#include "GeoPoint.hpp"

void TargetTrackerApp::create_observer()
{
  // Create the observer object:
  observer = new GeoObserver();

  // Since the observer is, by default, the current platform, then
  // connect the platform's movements to the observer's position
  // update method.
  observer_source = QGeoPositionInfoSource::createDefaultSource(this);
  if (observer_source){
    connect(observer_source, &QGeoPositionInfoSource::positionUpdated,
            observer,        &GeoObserver::onObserverPositionChanged);
  }else{
    qDebug()<<"Failed source";
  }
}

void TargetTrackerApp::create_target()
{
  // Create the target object:
  target = new GeoEntity();

  // Connect the target to a position info source:
  //
  // TODO: Provide a command line option to obtain target position
  // updates from stdin (default), a RESTful interface URL, or a DBUS
  // object.  for now, obtain target position updates from the logfile
  // position info source.
  LogFilePositionSource *target_source_from_log_file = new LogFilePositionSource(this);
  connect(target_source_from_log_file, &LogFilePositionSource::positionUpdated, target, &GeoEntity::setPosition);
  target_source = target_source_from_log_file;

  // Connect the position info source's error signal to terminate the
  // program:
  connect(target_source_from_log_file, qOverload<QGeoPositionInfoSource::Error>(&LogFilePositionSource::error), this, &TargetTrackerApp::onError);
}

TargetTrackerApp::TargetTrackerApp(QCoreApplication *app, int argc, char *argv[]) :
  m_app(app)
{
  Q_UNUSED(argc);
  Q_UNUSED(argv);
  QCoreApplication::setApplicationName(QCoreApplication::translate("main", "target-tracker"));
  QCoreApplication::setApplicationVersion(GIT_VERSION);

  // Create the observer and target
  create_observer();
  create_target();
  
  // Point the observer at the target
  observer->setTarget(target);

  // Print everyone's movements
  connect(observer, &GeoEntity::positionChanged,    this, &TargetTrackerApp::onObserverPositionChanged);
  connect(target,   &GeoEntity::positionChanged,    this, &TargetTrackerApp::onTargetPositionChanged);
  connect(observer, &GeoObserver::lookAngleChanged, this, &TargetTrackerApp::onLookAngleChanged);

  // Set the update interval and start the updates:
  //
  // TODO: update the intrval based upon object speeds and distances
  // OR upon a command line parameter
  observer_source->setUpdateInterval(500);
  target_source->setUpdateInterval(500);
}

void TargetTrackerApp::onObserverPositionChanged(QGeoPositionInfo const &position)
{
  QTextStream stream(stdout);
  stream << "   observer's location: " << position.coordinate().toString() << Qt::endl;
}

void TargetTrackerApp::onTargetPositionChanged(QGeoPositionInfo const &position)
{
  QTextStream stream(stdout);
  stream << "     target's location: " << position.coordinate().toString() << Qt::endl;
}

void TargetTrackerApp::onLookAngleChanged(LookAngle const &lookAngle)
{
  QTextStream stream(stdout);
  stream << "     azimuth to target: " << lookAngle.azimuth() << Qt::endl;
  stream << "   elevation to target: " << lookAngle.elevation() << Qt::endl;
}

void TargetTrackerApp::onPositionChanged(QGeoPositionInfo const &info)
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

void TargetTrackerApp::main()
{
  // Tell the position sources to start reporting updates:
  observer_source->startUpdates();
  target_source->startUpdates();
}

void TargetTrackerApp::onError(QGeoPositionInfoSource::Error error)
{
  Q_UNUSED(error);

  // stop the updates:
  observer_source->stopUpdates();
  target_source->stopUpdates();
  
  // clean up:
  delete observer_source;
  delete observer;
  delete target_source;
  delete target;
  
  // signal that we're done:
  emit finished();
}
