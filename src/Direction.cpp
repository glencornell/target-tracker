#include "Direction.hpp"

Direction::Direction() :
  m_azimuth(0.0),
  m_elevation(0.0)
{
}

Direction::Direction(float azimuth, float elevation) :
  m_azimuth(azimuth),
  m_elevation(elevation)
{
}

Direction::Direction(Direction const &other) :
  m_azimuth(other.m_azimuth),
  m_elevation(other.m_elevation)
{
}

Direction::~Direction()
{
}

Direction &Direction::operator=(Direction const &other)
{
  m_azimuth = other.m_azimuth;
  m_elevation = other.m_elevation;
  return *this;
}

bool Direction::operator==(Direction const &) const
{
  return false;
}

float Direction::azimuth() const
{
  return m_azimuth;
}

void Direction::setAzimuth(float azimuth)
{
  m_azimuth = azimuth;
}

float Direction::elevation() const
{
  return m_elevation;
}

void Direction::setElevation(float elevation)
{
  m_elevation = elevation;
}
