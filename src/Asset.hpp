#pragma once

#include <QObject>
#include <QUuid>
#include <QGeoPositionInfo>
#include <QGeoCoordinate>

// An asset is an observable item, moving or not, that observers can
// look at.  You can be an asset, too.  Each asset communicates its
// position information to you via some IPC, which is encapsulated in
// the QGeoPositionInfoSource.  This position information might be a
// MAVLINK message from a serial port or a gpsd message on DBUS.  Once
// this object is created, you must connect the
// QGeoPositionInfoSource's onPositionChanged signal to this object's
// onPositionChanged slot so that this object will receive updates to
// the position.

class Asset : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QUuid uuid MEMBER m_uuid READ uuid);

public:
  // the default constructor will assign a UUID to the asset. This
  // constructor is typically for creating an instance of yourself.
  // This constructor does not insert the object into an asset list.
  Asset(QObject * parent = nullptr);

  // This constructor is used to create a proxy of a remote asset. In
  // this case, the remote asset transmits its own self-generated
  // UUID. This constructor does not insert the new object into an
  // asset list.
  Asset(QObject * parent, QUuid const &uuid);

  // Manual getter/setter methods
  QGeoPositionInfo const &position() const;
  void setPosition(QGeoPositionInfo const &position);
  QUuid const &uuid() const;
  
signals:
  //  This signal is emitted when the asset's position changes.
  void positionChanged(QGeoPositionInfo const &position);

public slots:
  // Connect this slot to a QGeoPositionInfoSource to get position updates.
  void onPositionChanged(QGeoPositionInfo const &position);
  
private:
  QUuid            m_uuid;
  QGeoPositionInfo m_lastPosition;
};
