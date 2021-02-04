#include "GeoEntity.hpp"
#include "GeoObserver.hpp"
#include "LookAngle.hpp"

GeoObserver::GeoObserver(QObject *parent) :
  GeoEntity(parent),
  m_targetType(TARGET_NONE),
  m_entity(nullptr)
{
}

GeoObserver::GeoObserver(QUuid const &uuid) :
  GeoEntity(uuid),
  m_targetType(TARGET_NONE),
  m_entity(nullptr)
{
}

GeoObserver::~GeoObserver()
{
}

void GeoObserver::setTarget()
{
  if (m_targetType == TARGET_ENTITY && m_entity)
    disconnect (m_entity, &GeoEntity::positionChanged, this, &GeoObserver::onTargetPositionChanged);
  m_entity = nullptr;
  m_targetType = TARGET_NONE;
  emit targetChanged(m_targetType);
}

void GeoObserver::setTarget(QGeoCoordinate const coordinate)
{
  if (m_targetType == TARGET_ENTITY && m_entity)
    disconnect (m_entity, &GeoEntity::positionChanged, this, &GeoObserver::onTargetPositionChanged);
  m_coordinate = coordinate;
  m_targetType = TARGET_COORDINATE;
  calculateLookAngle();
  emit targetChanged(m_targetType);
}

void GeoObserver::setTarget(LookAngle const commanded_lookAngle)
{
  if (m_targetType == TARGET_ENTITY && m_entity)
    disconnect (m_entity, &GeoEntity::positionChanged, this, &GeoObserver::onTargetPositionChanged);
  m_commanded_lookAngle = commanded_lookAngle;
  m_targetType = TARGET_LOOK_ANGLE;
  calculateLookAngle();
  emit targetChanged(m_targetType);
}

void GeoObserver::setTarget(GeoEntity *entity)
{
  if (m_entity != entity) {
    if (m_targetType == TARGET_ENTITY && m_entity)
      disconnect (m_entity, &GeoEntity::positionChanged, this, &GeoObserver::onTargetPositionChanged);
    if (entity == nullptr) {
      m_entity = nullptr;
      m_targetType = TARGET_NONE;
    } else {
      m_entity = entity;
      m_targetType = TARGET_ENTITY;
    
      // connect signals emitted from target
      connect (m_entity, &GeoEntity::positionChanged, this, &GeoObserver::onTargetPositionChanged);
      calculateLookAngle();
      emit targetChanged(m_targetType);
    }
  }
}

LookAngle GeoObserver::lookAngle() const
{
  return m_lookAngle;
}

void GeoObserver::calculateLookAngle()
{
  // For readability, create the observer (which is this object instance):
  GeoEntity *observer = this;
  LookAngle next;
  
  switch (m_targetType)
    {
    case TARGET_ENTITY:
      next.setLookAngle(observer->position().coordinate(),
                        m_entity->position().coordinate());
      break;
    case TARGET_COORDINATE:
      next.setLookAngle(observer->position().coordinate(),
                        m_coordinate);
      break;
    case TARGET_LOOK_ANGLE:
      next = m_commanded_lookAngle;
      emit lookAngleChanged(m_lookAngle);
    default:
      break;
    }
  // TODO: only set if the look angle has changed:
  m_lookAngle = next;
  emit lookAngleChanged(m_lookAngle);
}

void GeoObserver::positionChanged(QGeoPositionInfo const &position)
{
  GeoEntity::positionChanged(position);
  calculateLookAngle();
}

void GeoObserver::onTargetPositionChanged(QGeoPositionInfo const &)
{
  calculateLookAngle();
}
