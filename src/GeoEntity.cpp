#include "GeoEntity.hpp"

GeoEntity::GeoEntity(QObject * parent) :
  QObject(parent),
  m_uuid(QUuid::createUuid()),
  m_positionSource(nullptr),
  m_position(),
  m_rotationSource(nullptr),
  m_rotation(new QRotationReading(this)),
  m_started(false)
{
}

GeoEntity::GeoEntity(QUuid const &uuid) :
  QObject(),
  m_uuid(uuid),
  m_positionSource(nullptr),
  m_position(),
  m_rotationSource(nullptr),
  m_rotation(new QRotationReading(this)),
  m_started(false)
{
}

GeoEntity::~GeoEntity()
{
  delete m_rotation;
}

QUuid const GeoEntity::uuid() const
{
  return m_uuid;
}

void GeoEntity::setPositionSource(QGeoPositionInfoSource *src)
{
  Q_ASSERT(src);
  if (m_positionSource)
    disconnect(m_positionSource, &QGeoPositionInfoSource::positionUpdated, this, &GeoEntity::setPosition);
  m_positionSource = src;
  connect(m_positionSource, &QGeoPositionInfoSource::positionUpdated, this, &GeoEntity::setPosition);
  if(m_started)
    m_positionSource->startUpdates();
}

QGeoPositionInfoSource *GeoEntity::positionSource() const
{
  return m_positionSource;
}

void GeoEntity::setPosition(QGeoPositionInfo const &position)
{
  m_position = position;
  emit positionChanged(m_position);
}

QGeoPositionInfo const GeoEntity::position() const
{
  return m_position;
}

void GeoEntity::setRotationSource(RotationReadingSource *src)
{
  Q_ASSERT(src);
  if (m_rotationSource)
    disconnect(m_rotationSource, &RotationReadingSource::rotationChanged, this, &GeoEntity::setRotation);
  m_rotationSource = src;
  connect(m_rotationSource, &RotationReadingSource::rotationChanged, this, &GeoEntity::setRotation);
  if(m_started)
    m_rotationSource->startUpdates();
}

RotationReadingSource *GeoEntity::rotationSource() const
{
  return m_rotationSource;
}

QRotationReading *GeoEntity::rotation() const
{
  return m_rotation;
}

void GeoEntity::setRotation(QRotationReading *reading)
{
  m_rotation->setTimestamp(reading->timestamp());
  m_rotation->setFromEuler(reading->x(), reading->y(), reading->z());
  emit rotationChanged(m_rotation);
}

void GeoEntity::startUpdates()
{
  // Call startUpdates() on all children
  for (auto child : children())
    {
      GeoEntity *entity = qobject_cast<GeoEntity *>(child);
      if (entity)
        entity->startUpdates();
    }
  if (m_positionSource)
    m_positionSource->startUpdates();
  if (m_rotationSource)
    m_rotationSource->startUpdates();
  m_started = true;
}
