#pragma once

#include <QObject>
#include <QUuid>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <QRotationSensor>
#include "RotationReadingSource.hpp"
#include <QRotationReading>

// A GeoEntity is an object in the physical world, moving or not.
// Each GeoEntity has an unique identifier, a geographic position and
// a rotation. GeoEntities obtain position and rotation (a.k.a. pose)
// information from data source objects injected at object
// construction.  Position sources may be MAVLINK messages from a
// serial port, a gpsd UDP message, a GeoClue2 DBUS message, a physics
// engine, or even a text file.  If the rotational source is not
// provided, then the GeoEntity assumes a (0,0,0) rotation.

class GeoEntity : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QGeoPositionInfo  position READ position NOTIFY positionChanged)
  Q_PROPERTY(QRotationReading *rotation READ rotation NOTIFY rotationChanged)
public:
  GeoEntity(QObject *parent = nullptr);
  GeoEntity(QUuid const &uuid);
  virtual ~GeoEntity();

  QUuid const uuid() const;

  void setPositionSource(QGeoPositionInfoSource *src);
  QGeoPositionInfoSource *positionSource() const;
  QGeoPositionInfo const position() const;

  void setRotationSource(RotationReadingSource *src);
  RotationReadingSource *rotationSource() const;
  QRotationReading *rotation() const;

signals:
  void positionChanged(QGeoPositionInfo const &position);
  void rotationChanged(QRotationReading *rotation);

public slots:
  // Start receiving values from the sensors. You will not get
  // position/oriententation updates until this method is invoked.
  // This method implements the delegate of responsibility pattern to
  // call the position and rotation source startUpdates() methods.
  virtual void startUpdates();

  // This slot is used to connect a QGeoPositionInfoSource to get
  // position updates.
  void setPosition(QGeoPositionInfo const &position);

  // This slot is called by the RotationReadingSource whenever new
  // values are available.
  void setRotation(QRotationReading *reading);
  
private:
  QUuid                   m_uuid;

  QGeoPositionInfoSource *m_positionSource;
  QGeoPositionInfo        m_position;

  RotationReadingSource  *m_rotationSource;
  QRotationReading       *m_rotation;

  bool                    m_started;
};
