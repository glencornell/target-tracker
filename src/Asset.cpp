#include "Asset.hpp"

Asset::Asset(QObject * parent) :
  QObject(parent),
  m_uuid(QUuid::createUuid()),
  m_rotation(new QRotationReading(this)),
  m_started(false)
{
}

Asset::Asset(QUuid const &uuid) :
  m_uuid(uuid),
  m_rotation(new QRotationReading(this)),
  m_started(false)
{
}

Asset::~Asset()
{
  delete m_rotation;
}

QUuid const Asset::uuid() const
{
  return m_uuid;
}

void Asset::setPositionSource(QGeoPositionInfoSource *src)
{
  Q_ASSERT(src);
  // if (m_positionSource)
  //   disconnect(m_positionSource, &QGeoPositionInfoSource::positionUpdated, this, &Asset::setPosition);
  m_positionSource = src;
  connect(m_positionSource, &QGeoPositionInfoSource::positionUpdated, this, &Asset::setPosition);
  if(m_started)
    m_positionSource->startUpdates();
}

QGeoPositionInfoSource *Asset::positionSource() const
{
  return m_positionSource;
}

void Asset::setPosition(QGeoPositionInfo const &position)
{
  m_position = position;
  emit positionChanged(m_position);
}

QGeoPositionInfo const Asset::position() const
{
  return m_position;
}

void Asset::setRotationSource(RotationReadingSource *src)
{
  Q_ASSERT(src);
  // if (m_rotationSource)
  //   disconnect(m_rotationSource, &RotationReadingSource::rotationChanged, this, &Asset::setRotation);
  m_rotationSource = src;
  connect(m_rotationSource, &RotationReadingSource::rotationChanged, this, &Asset::setRotation);
  if(m_started)
    m_rotationSource->startUpdates();
}

RotationReadingSource *Asset::rotationSource() const
{
  return m_rotationSource;
}

QRotationReading *Asset::rotation() const
{
  return m_rotation;
}

void Asset::setRotation(QRotationReading *reading)
{
  m_rotation->setTimestamp(reading->timestamp());
  m_rotation->setFromEuler(reading->x(), reading->y(), reading->z());
  emit rotationChanged(m_rotation);
}

void Asset::startUpdates()
{
  // Call startUpdates() on all children
  for (auto child : children())
    {
      Asset *asset = qobject_cast<Asset *>(child);
      if (asset)
        asset->startUpdates();
    }
  if (m_positionSource)
    m_positionSource->startUpdates();
  if (m_rotationSource)
    m_rotationSource->startUpdates();
  m_started = true;
}
