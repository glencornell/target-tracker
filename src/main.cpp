#include <QCoreApplication>
#include <QGeoCoordinate>
#include <QTextStream>
#include "look-at.hpp"

int main(int argc, char * argv[]) {
  QCoreApplication a(argc, argv);
  QTextStream stream(stdout);
  QGeoCoordinate point_a(34.24333333, 118.0975000, 1877.873);
  QGeoCoordinate point_b(34.24666667, 118.1038889, 1816.608);

  double distance;
  double azimuth;
  double elevation;

  lookAt(point_a, point_b, true, &distance, &azimuth, &elevation);

  stream << "point_a: " << point_a.toString() << Qt::endl;
  stream << "point_b: " << point_b.toString() << Qt::endl;
  stream << " distance to point b: " << distance << Qt::endl;
  stream << "  azimuth to point b: " << azimuth << Qt::endl;
  stream << "elevation to point b: " << elevation << Qt::endl;

  // Main loop:
  return a.exec();
}
