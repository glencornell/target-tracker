#include "GeoTarget.hpp"

GeoTarget::GeoTarget() :
  m_targetType(TARGET_NONE)
{
}

GeoTarget::GeoTarget(QGeoCoordinate coordinate) :
  m_targetType(TARGET_COORDINATE),
  m_coordinate(coordinate)
{
}

GeoTarget::GeoTarget(GeoEntity *entity) :
  m_targetType(TARGET_ENTITY),
  m_entity(entity)
{
}

GeoTarget::GeoTarget(LookAngle lookAngle) :
  m_targetType(TARGET_LOOK_ANGLE),
  m_lookAngle(lookAngle)
{
}

GeoTarget::GeoTarget(GeoTarget const &other) :
  m_targetType(other.m_targetType)
{
  switch(m_targetType) {
  case TARGET_LOOK_ANGLE:
    m_lookAngle = other.m_lookAngle;
    break;
  case TARGET_COORDINATE:
    m_coordinate = other.m_coordinate;
    break;
  case TARGET_ENTITY:
    m_entity = other.m_entity;
    break;
  default:
    break;
  }
}

GeoTarget::~GeoTarget()
{
}

GeoTarget &GeoTarget::operator=(GeoTarget const &other)
{
  m_targetType = other.m_targetType;
  switch(m_targetType) {
  case TARGET_LOOK_ANGLE:
    m_lookAngle = other.m_lookAngle;
    break;
  case TARGET_COORDINATE:
    m_coordinate = other.m_coordinate;
    break;
  case TARGET_ENTITY:
    m_entity = other.m_entity;
    break;
  default:
    break;
  }
  return *this;
}

bool GeoTarget::operator==(const GeoTarget &other) const
{
  if (m_targetType == other.m_targetType)
    {
      switch(m_targetType) {
      case TARGET_LOOK_ANGLE:
        return m_lookAngle == other.m_lookAngle;
        break;
      case TARGET_COORDINATE:
        return m_coordinate == other.m_coordinate;
        break;
      case TARGET_ENTITY:
        return m_entity == other.m_entity;
        break;
      default:
        break;
      }
    }
  return false;
}

GeoTarget::TargetType GeoTarget::targetType() const
{
  return m_targetType;
}

LookAngle GeoTarget::lookAngle() const
{
  return m_lookAngle;
}

void GeoTarget::setLookAngle(LookAngle lookAngle)
{
  m_targetType = TARGET_LOOK_ANGLE;
  m_lookAngle  = lookAngle;
}

QGeoCoordinate GeoTarget::coordinate() const
{
  return m_coordinate;
}

void GeoTarget::setCoordinate(QGeoCoordinate coordinate)
{
  m_targetType = TARGET_COORDINATE;
  m_coordinate  = coordinate;
}

GeoEntity *GeoTarget::entity() const
{
  return m_entity;
}

void GeoTarget::setEntity(GeoEntity *entity)
{
  m_targetType = TARGET_ENTITY;
  m_entity  = entity;
}

bool GeoTarget::isNothing() const
{
  return m_targetType == TARGET_NONE;
}

bool GeoTarget::isLookAngle() const
{
  return m_targetType == TARGET_LOOK_ANGLE;
}

bool GeoTarget::isCoordinate() const
{
  return m_targetType == TARGET_COORDINATE;
}

bool GeoTarget::isEntity() const
{
  return m_targetType == TARGET_ENTITY;
}
