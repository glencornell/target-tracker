#include <QtGlobal>
#include <QDebug>
#include "RotationReadingSource.hpp"

RotationReadingSource::RotationReadingSource(QObject *parent)
  : QObject(parent),
    m_rotation(new QRotationReading)
{
  // connect to the sensor backend
  if (!m_rotationSensor.connectToBackend())
    {
      qWarning() << "Cannot connect to rotation sensor backend!";
    }
  
  // ... and then add a filter that will process the read data
  m_rotationSensor.addFilter(this);
}

void RotationReadingSource::startUpdates()
{
  // Start gathering the data
  m_rotationSensor.start();
}

QRotationReading *RotationReadingSource::rotation() const
{
  return m_rotation;
}

bool RotationReadingSource::filter(QRotationReading *reading)
{
  m_rotation->setTimestamp(reading->timestamp());
  m_rotation->setFromEuler(reading->x(), reading->y(), reading->z());
  emit rotationChanged(m_rotation);

  // Do no further processing of the sensor data
  return false;
}
