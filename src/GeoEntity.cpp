#include "GeoEntity.hpp"

GeoEntity::GeoEntity(QObject * parent) :
  QObject(parent),
  m_uuid(QUuid::createUuid()),
  m_position(),
  m_rotation(new QRotationReading(this))
{
}

GeoEntity::GeoEntity(QUuid const &uuid) :
  QObject(),
  m_uuid(uuid),
  m_position(),
  m_rotation(new QRotationReading(this))

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

void GeoEntity::setPosition(QGeoPositionInfo const &position)
{
  m_position = position;
  emit positionChanged(m_position);
}

QGeoPositionInfo const GeoEntity::position() const
{
  return m_position;
}

QRotationReading *GeoEntity::rotation() const
{
  return m_rotation;
}

void GeoEntity::setRotation(QRotationReading const *reading)
{
  m_rotation->setTimestamp(reading->timestamp());
  m_rotation->setFromEuler(reading->x(), reading->y(), reading->z());
  emit rotationChanged(m_rotation);
}
