#include <QCoreApplication>
#include <QGeoCoordinate>
#include <QTimer>
#include "GeoObserver.hpp"

int main(int argc, char * argv[]) {
  QCoreApplication a(argc, argv);
  QGeoCoordinate point_a(34.24333333, 118.0975000, 1877.873);
  QGeoCoordinate point_b(34.24666667, 118.1038889, 1816.608);
  GeoObserver observer;
  
  observer.setObserver(point_a);
  observer.setObserved(point_b);

  // quit application when work is complete:
  QObject::connect(&observer, SIGNAL(finished()), &a, SLOT(quit()));

  // Run the observer's user hook in the main loop:
  QTimer::singleShot(0, &observer, SLOT(getLookAngle()));

  // Run the main loop:
  return a.exec();
}
