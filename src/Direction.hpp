#pragma once

#include <QtCore/QMetaType>

class Direction
{
  Q_GADGET
  
  Q_PROPERTY(float azimuth READ azimuth WRITE setAzimuth)
  Q_PROPERTY(float elevation READ elevation WRITE setElevation)
  
public:
  
  Direction();
  Direction(float azimuth, float elevation);
  Direction(Direction const &other);
  ~Direction();

  Direction &operator=(Direction const &other);
  bool operator==(const Direction &other) const;
  inline bool operator!=(const Direction &other) const {
    return !operator==(other);
  }
  
  float azimuth() const;
  void setAzimuth(float azimuth);
  
  float elevation() const;
  void setElevation(float elevation);
  
private:  
  float m_azimuth;   // degrees from true north
  float m_elevation; // degrees from the horizon
};
