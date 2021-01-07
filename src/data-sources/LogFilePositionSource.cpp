/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
// taken from https://doc.qt.io/archives/qt-5.8/qtpositioning-logfilepositionsource-logfilepositionsource-cpp.html
#include <QtCore>
#include "LogFilePositionSource.hpp"

LogFilePositionSource::LogFilePositionSource(QObject *parent)
  : QGeoPositionInfoSource(parent),
    logFile(new QFile(this)),
    timer(new QTimer(this))
{
  connect(timer, SIGNAL(timeout()), this, SLOT(readNextPosition()));

  //  logFile->setFileName(":/sim-data/log-long.txt");
  logFile->setFileName(":/sim-data/log-short.txt");
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
