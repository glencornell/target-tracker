#include <QCoreApplication>
#include <QGeoCoordinate>
#include <QTextStream>

int main(int argc, char * argv[]) {
  QCoreApplication a(argc, argv);
  QTextStream stream(stdout);
  QGeoCoordinate point_a;
  QGeoCoordinate point_b;

  point_a.setLatitude(87.1234);
  point_a.setLongitude(-14.1234);
  
  point_b.setLatitude(47.1234);
  point_b.setLongitude(-16.1234);

  qreal distance = point_a.distanceTo(point_b);
  qreal azimuth = point_a.azimuthTo(point_b);

  stream << "point_a: " << point_a.toString() << endl;
  stream << "point_b: " << point_b.toString() << endl;
  stream << "distance to point b: " << distance << endl;
  stream << " azimuth to point b: " << azimuth << endl;

  // Main loop:
  return a.exec();
}
