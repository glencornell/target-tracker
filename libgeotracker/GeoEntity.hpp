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
  Q_PROPERTY(QGeoPositionInfo  position READ position WRITE setPosition NOTIFY positionChanged)
  Q_PROPERTY(QRotationReading *rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
public:
  GeoEntity(QObject *parent = nullptr);
  GeoEntity(QUuid const &uuid);
  virtual ~GeoEntity();

  QUuid const uuid() const;

  QGeoPositionInfo const position() const;
  QRotationReading *rotation() const;

signals:
  void positionChanged(QGeoPositionInfo const &position);
  void rotationChanged(QRotationReading *rotation);

public slots:
  // This slot is used to connect a QGeoPositionInfoSource to get
  // position updates.
  virtual void setPosition(QGeoPositionInfo const &position);

  // This slot is called by the RotationReadingSource whenever new
  // values are available.
  virtual void setRotation(QRotationReading const *reading);
  
private:
  QUuid                   m_uuid;
  QGeoPositionInfo        m_position;
  QRotationReading       *m_rotation;
};
