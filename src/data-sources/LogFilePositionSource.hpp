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
