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
// taken from https://doc.qt.io/archives/qt-5.8/qtpositioning-logfilepositionsource-logfilepositionsource-h.html

#pragma once

#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <QFile>
#include <QTimer>

// The Logfile Position Source shows how to create and work with a
// custom NMEA position source, for platforms without GPS.
//
// The data is read from a file which has positional data in NMEA
// format. The resulting time and position information is then
// displayed to the screen as simple text in date/time and
// latitude/longitude format.
//
// This example class reads position data from a text file,
// log.txt. The file specifies position data using a simple text
// format: it contains one position update per line, where each line
// contains a date/time, a latitude, a longitude and an altitude,
// separated by spaces. The date/time is in ISO 8601 format and the
// latitude and longitude are in degrees decimal format. Here is an
// excerpt from log.txt:
//
// 2009-08-24T22:25:01 -27.576082 153.092415 1180.0
// 2009-08-24T22:25:02 -27.576223 153.092530 1181.0
// 2009-08-24T22:25:03 -27.576364 153.092648 1182.0
//
// The class reads this data and distributes it via the
// positionUpdated() signal.

class LogFilePositionSource : public QGeoPositionInfoSource
{
  Q_OBJECT
public:
  LogFilePositionSource(QObject *parent = 0);
  
  QGeoPositionInfo lastKnownPosition(bool fromSatellitePositioningMethodsOnly = false) const;
  
  PositioningMethods supportedPositioningMethods() const;
  int minimumUpdateInterval() const;
  Error error() const;

signals:
  void error(QGeoPositionInfoSource::Error e);
                     
public slots:
  virtual void startUpdates();
  virtual void stopUpdates();
  
  virtual void requestUpdate(int timeout = 5000);
                                                
private slots:
  void readNextPosition();
                         
private:
  QFile *logFile;
  QTimer *timer;
  QGeoPositionInfo lastPosition;
};
