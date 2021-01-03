#include "Target.hpp"

Target::Target() :
  m_targetType(TARGET_NONE)
{
}

Target::Target(QGeoCoordinate position) :
  m_targetType(TARGET_POSITION),
  m_position(position)
{
}

Target::Target(Asset *asset) :
  m_targetType(TARGET_ASSET),
  m_asset(asset)
{
}

Target::Target(Direction direction) :
  m_targetType(TARGET_DIRECTION),
  m_direction(direction)
{
}

Target::Target(Target const &other) :
  m_targetType(other.m_targetType)
{
  switch(m_targetType) {
  case TARGET_DIRECTION:
    m_direction = other.m_direction;
    break;
  case TARGET_POSITION:
    m_position = other.m_position;
    break;
  case TARGET_ASSET:
    m_asset = other.m_asset;
    break;
  default:
    break;
  }
}

Target::~Target()
{
}

Target &Target::operator=(Target const &other)
{
  m_targetType = other.m_targetType;
  switch(m_targetType) {
  case TARGET_DIRECTION:
    m_direction = other.m_direction;
    break;
  case TARGET_POSITION:
    m_position = other.m_position;
    break;
  case TARGET_ASSET:
    m_asset = other.m_asset;
    break;
  default:
    break;
  }
  return *this;
}

bool Target::operator==(const Target &other) const
{
  if (m_targetType == other.m_targetType)
    {
      switch(m_targetType) {
      case TARGET_DIRECTION:
        return m_direction == other.m_direction;
        break;
      case TARGET_POSITION:
        return m_position == other.m_position;
        break;
      case TARGET_ASSET:
        return m_asset == other.m_asset;
        break;
      default:
        break;
      }
    }
  return false;
}

Target::TargetType Target::targetType() const
{
  return m_targetType;
}

Direction Target::direction() const
{
  return m_direction;
}

QGeoCoordinate Target::position() const
{
  return m_position;
}

Asset *Target::asset() const
{
  return m_asset;
}
