#include "GeoEntity.hpp"
#include "GeoObserver.hpp"
#include "look-at.hpp"

GeoObserver::GeoObserver(GeoEntity *parent) :
  GeoEntity(parent),
  m_target(new GeoTarget)
{
  if (parent)
    connect(parent, &GeoEntity::positionChanged, this, &GeoObserver::onObserverPositionChanged);
}

GeoObserver::GeoObserver(QUuid const &uuid) :
  GeoEntity(uuid),
  m_target(new GeoTarget)
{
}

GeoObserver::~GeoObserver()
{
}

void GeoObserver::setParent(GeoEntity *parent)
{
  if (parent)
    {
      if (GeoEntity *old_parent = qobject_cast<GeoEntity *>(this->parent()))
        disconnect(old_parent, &GeoEntity::positionChanged, this, &GeoObserver::onObserverPositionChanged);
      connect(parent, &GeoEntity::positionChanged, this, &GeoObserver::onObserverPositionChanged);
    }
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

Direction GeoObserver::lookAngle() const
{
  return m_lookAngle;
}

void GeoObserver::setLookAngle()
{
  // lookAt return parameters:
  double distance;
  double azimuth;
  double elevation;
  
  // Set up the observer's position (first from the parent)
  GeoEntity *observer = qobject_cast<GeoEntity *>(parent());
  if (!observer)
    {
      // If this is not attached to anything, then use your own
      // position as the observation point.
      observer = this;
    }
  
  switch (m_target->targetType())
    {
    case GeoTarget::TARGET_ENTITY:
      ::lookAt(observer->position().coordinate(),
               m_target->entity()->position().coordinate(),
               &distance, &azimuth, &elevation);
      m_lookAngle = Direction(azimuth, elevation);
      emit lookAngleChanged(m_lookAngle);
      break;
    case GeoTarget::TARGET_COORDINATE:
      ::lookAt(observer->position().coordinate(),
               m_target->coordinate(),
               &distance, &azimuth, &elevation);
      m_lookAngle = Direction(azimuth, elevation);
      emit lookAngleChanged(m_lookAngle);
      break;
    case GeoTarget::TARGET_DIRECTION:
      m_lookAngle = m_target->direction();
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
