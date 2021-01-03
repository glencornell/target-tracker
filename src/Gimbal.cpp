#include "Gimbal.hpp"
#include "Asset.hpp"
#include "look-at.hpp"

Gimbal::Gimbal(Asset *parent) :
  Asset(parent),
  m_target(new Target)
{
  if (parent)
    connect(parent, &Asset::positionChanged, this, &Gimbal::onObserverPositionChanged);
}

Gimbal::Gimbal(QUuid const &uuid) :
  Asset(uuid),
  m_target(new Target)
{
}

Gimbal::~Gimbal()
{
}

void Gimbal::setParent(Asset *parent)
{
  if (parent)
    {
      if (Asset *old_parent = qobject_cast<Asset *>(this->parent()))
        disconnect(old_parent, &Asset::positionChanged, this, &Gimbal::onObserverPositionChanged);
      connect(parent, &Asset::positionChanged, this, &Gimbal::onObserverPositionChanged);
    }
}

Target *Gimbal::target() const
{
  return m_target;
}

void Gimbal::setTarget(Target *target)
{
  QGeoPositionInfo dummy;
  if (m_target)
    {
      if (m_target->targetType() == Target::TARGET_ASSET)
        {
          disconnect (m_target->asset(), &Asset::positionChanged, this, &Gimbal::onTargetPositionChanged);
        }
      delete m_target;
    }
  m_target = target;
  emit targetChanged();

  setLookAngle();
  
  // connect signals emitted from target
  if (m_target->targetType() == Target::TARGET_ASSET)
    {
      connect (m_target->asset(), &Asset::positionChanged, this, &Gimbal::onTargetPositionChanged);
    }
}

Direction Gimbal::lookAngle() const
{
  return m_lookAngle;
}

void Gimbal::setLookAngle()
{
  switch (m_target->targetType())
    {
    case Target::TARGET_ASSET:
      {
        Asset *observer = qobject_cast<Asset *>(parent());
        if (observer)
          {
            double distance;
            double azimuth;
            double elevation;
            
            lookAt(observer->position().coordinate(), m_target->asset()->position().coordinate(), &distance, &azimuth, &elevation);
            m_lookAngle = Direction(azimuth, elevation);
            emit lookAngleChanged(m_lookAngle);
          }
      }
      break;
    default:
      break;
    }
}

void Gimbal::onObserverPositionChanged(QGeoPositionInfo const &)
{
  setLookAngle();
}

void Gimbal::onTargetPositionChanged(QGeoPositionInfo const &)
{
  setLookAngle();
}
