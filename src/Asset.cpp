#include "Asset.hpp"

// the default constructor will assign a UUID to the asset. This
// constructor is typically for creating an instance of yourself.
// This constructor does not insert the object into an asset list.
Asset::Asset(__attribute__ ((unused)) QObject * parent) :
  m_uuid(QUuid::createUuid())
{
}

// This constructor is used to create a proxy of a remote asset. In
// this case, the remote asset transmits its own self-generated
// UUID. This constructor does not insert the new object into an
// asset list.
Asset::Asset(__attribute__ ((unused)) QObject * parent, QUuid const &uuid) :
  m_uuid(uuid)
{
}

// Manual getter/setter methods
QGeoPositionInfo const &Asset::position() const
{
  return m_lastPosition;
}

void Asset::setPosition(QGeoPositionInfo const &position)
{
  m_lastPosition = position;
  emit positionChanged(m_lastPosition);
}

QUuid const &Asset::uuid() const
{
  return m_uuid;
}

// Connect this slot to a QGeoPositionInfoSource to get position updates.
void Asset::onPositionChanged(QGeoPositionInfo const &position)
{
  m_lastPosition = position;
  emit positionChanged(m_lastPosition);
}
