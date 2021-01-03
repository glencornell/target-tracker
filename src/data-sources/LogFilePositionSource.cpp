#include <QtCore>
#include "LogFilePositionSource.hpp"

LogFilePositionSource::LogFilePositionSource(QObject *parent)
  : QGeoPositionInfoSource(parent),
    logFile(new QFile(this)),
    timer(new QTimer(this))
{
  connect(timer, SIGNAL(timeout()), this, SLOT(readNextPosition()));

  logFile->setFileName(":/log-short.txt");
  if (!logFile->open(QIODevice::ReadOnly))
    qWarning() << "Error: cannot open source file" << logFile->fileName();
}

QGeoPositionInfo LogFilePositionSource::lastKnownPosition(bool /*fromSatellitePositioningMethodsOnly*/) const
{
  return lastPosition;
}

LogFilePositionSource::PositioningMethods LogFilePositionSource::supportedPositioningMethods() const
{
  return AllPositioningMethods;
}

int LogFilePositionSource::minimumUpdateInterval() const
{
  return 500;
}

void LogFilePositionSource::startUpdates()
{
  int interval = updateInterval();
  if (interval < minimumUpdateInterval())
    interval = minimumUpdateInterval();

  timer->start(interval);
}

void LogFilePositionSource::stopUpdates()
{
  timer->stop();
}

void LogFilePositionSource::requestUpdate(int /* timeout */)
{
  if (logFile->canReadLine())
    readNextPosition();
  else
    emit updateTimeout();
}

void LogFilePositionSource::readNextPosition()
{
  QByteArray line = logFile->readLine().trimmed();
  if (line.isEmpty()) {
    emit error(QGeoPositionInfoSource::ClosedError);
  } else {
    QList<QByteArray> data = line.split(' ');
    double latitude;
    double longitude;
    double altitude;
    bool hasLatitude = false;
    bool hasLongitude = false;
    bool hasAltitude = false;
    QDateTime timestamp = QDateTime::fromString(QString(data.value(0)), Qt::ISODate);
    latitude = data.value(1).toDouble(&hasLatitude);
    longitude = data.value(2).toDouble(&hasLongitude);
    altitude = data.value(3).toDouble(&hasAltitude);

    if (hasLatitude && hasLongitude && hasAltitude && timestamp.isValid()) {
      QGeoCoordinate coordinate(latitude, longitude, altitude);
      QGeoPositionInfo info(coordinate, timestamp);
      if (info.isValid()) {
        lastPosition = info;
        emit positionUpdated(info);
      }
    }
  }
}

QGeoPositionInfoSource::Error LogFilePositionSource::error() const
{
  return QGeoPositionInfoSource::ClosedError;
}
