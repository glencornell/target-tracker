#pragma once

#include <QObject>
#include <QUuid>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <QRotationSensor>
#include "RotationReadingSource.hpp"
#include <QRotationReading>

// An asset is an object in the physical world, moving or not.  Each
// Asset has an unique identifier, a geographic position and a
// rotation. Assets obtain position and rotation (a.k.a. pose)
// information from data source objects injected at object
// construction.  Position sources may be MAVLINK messages from a
// serial port, a gpsd UDP message, a GeoClue2 DBUS message, a physics
// engine, or even a text file.  If the rotational source is not
// provided, then the Asset assumes a (0,0,0) rotation.

class Asset : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QGeoPositionInfo position READ position NOTIFY positionChanged)
  Q_PROPERTY(QRotationReading *rotation READ rotation NOTIFY rotationChanged)
public:
  Asset(QObject *parent = nullptr);
  Asset(QUuid const &uuid);
  virtual ~Asset();

  QUuid const uuid() const;

  void setPositionSource(QGeoPositionInfoSource *src);
  QGeoPositionInfoSource *positionSource() const;
  QGeoPositionInfo const position() const;

  void setRotationSource(RotationReadingSource *src);
  RotationReadingSource *rotationSource() const;
  QRotationReading *rotation() const;

signals:
  // emitted when the asset's position changes.
  void positionChanged(QGeoPositionInfo const &position);

  // emitted when the asset's rotation changes.
  void rotationChanged(QRotationReading *reading);

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
