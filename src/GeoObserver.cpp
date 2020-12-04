#include <QTextStream>
#include "GeoObserver.hpp"
#include "look-at.hpp"

GeoObserver::GeoObserver(__attribute__ ((unused)) QObject *parent)
{
}

void GeoObserver::setObserver(QGeoCoordinate &observer)
{
  m_observer = observer;
}

void GeoObserver::setObserved(QGeoCoordinate &observed)
{
  m_observed = observed;
}

void GeoObserver::getLookAngle()
{
  QTextStream stream(stdout);
  double distance;
  double azimuth;
  double elevation;

  lookAt(m_observer, m_observed, &distance, &azimuth, &elevation);

  stream << " observer's location: " << m_observer.toString() << Qt::endl;
  stream << " observed's location: " << m_observed.toString() << Qt::endl;
  stream << " distance to point b: " << distance << Qt::endl;
  stream << "  azimuth to point b: " << azimuth << Qt::endl;
  stream << "elevation to point b: " << elevation << Qt::endl;

  // signal that we're done:
  emit finished();
}
