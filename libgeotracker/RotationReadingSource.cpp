#include <QtGlobal>
#include <QDebug>
#include "RotationReadingSource.hpp"

RotationReadingSource::RotationReadingSource(QObject *parent)
  : QObject(parent),
    m_rotation(new QRotationReading)
{
  // connect to the default sensor backend
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

  // TODO: rather than pass all data unconditionally, compare the
  // current reading against the last reported reading: if the
  // difference exceeds a threshold, then emit the signal.
  emit rotationChanged(m_rotation);

  // Do no further processing of the sensor data
  return false;
}
