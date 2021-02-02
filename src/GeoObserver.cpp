#include "GeoEntity.hpp"
#include "GeoObserver.hpp"
#include "LookAngle.hpp"

GeoObserver::GeoObserver(QObject *parent) :
  GeoEntity(parent),
  m_target(new GeoTarget)
{
}

GeoObserver::GeoObserver(QUuid const &uuid) :
  GeoEntity(uuid),
  m_target(new GeoTarget)
{
}

GeoObserver::~GeoObserver()
{
}

GeoTarget *GeoObserver::target() const
{
  return m_target;
}

void GeoObserver::setTarget(GeoTarget *target)
{
  QGeoPositionInfo dummy;
  if (m_target)
    {
      if (m_target->isEntity())
        {
          disconnect (m_target->entity(), &GeoEntity::positionChanged, this, &GeoObserver::onTargetPositionChanged);
        }
      delete m_target;
    }
  m_target = target;
  emit targetChanged();

  setLookAngle();
  
  // connect signals emitted from target
  if (m_target->isEntity())
    {
      connect (m_target->entity(), &GeoEntity::positionChanged, this, &GeoObserver::onTargetPositionChanged);
    }
}

LookAngle GeoObserver::lookAngle() const
{
  return m_lookAngle;
}

void GeoObserver::setLookAngle()
{
  // For readability, this instance is the observer
  GeoEntity *observer = this;
  
  switch (m_target->targetType())
    {
    case GeoTarget::TARGET_ENTITY:
      m_lookAngle.setLookAngle(observer->position().coordinate(),
                               m_target->entity()->position().coordinate());
      emit lookAngleChanged(m_lookAngle);
      break;
    case GeoTarget::TARGET_COORDINATE:
      m_lookAngle.setLookAngle(observer->position().coordinate(),
                               m_target->coordinate());
      emit lookAngleChanged(m_lookAngle);
      break;
    case GeoTarget::TARGET_LOOK_ANGLE:
      // TODO: only set if the look angle has changed:
      m_lookAngle = m_target->lookAngle();
      emit lookAngleChanged(m_lookAngle);
    default:
      break;
    }
}

void GeoObserver::onObserverPositionChanged(QGeoPositionInfo const &)
{
  setLookAngle();
}

void GeoObserver::onTargetPositionChanged(QGeoPositionInfo const &)
{
  setLookAngle();
}
