#include <QTextStream>
#include <QDateTime>
#include <QGeoCoordinate>
#include <QCommandLineParser>
#include "LACApp.hpp"
#include "LookAngle.hpp"
#include "GeoPoint.hpp"

void LACApp::help(){
  QTextStream out(stdout);
  out << "This program calculates the look angle from the observer to the" << Qt::endl
      << "target.  Coordinates are defined by latitude in decimal degrees," << Qt::endl
      << "longitude in decimal degrees, and optionally, altitude in meters above" << Qt::endl
      << "sea level.  The coordinate values should be specified using the WGS84" << Qt::endl
      << "datum. The look angle is a line of sight calculation, not the geodetic" << Qt::endl
      << "problem.  Azimuth in this context is represented in degrees from true" << Qt::endl
      << "north to the target while elevation is the angle in degrees from the" << Qt::endl
      << "horizon to the target. To be valid, the latitude must be between -90" << Qt::endl
      << "to 90 inclusive. To be valid, the longitude must be between -180 to" << Qt::endl
      << "180 inclusive." << Qt::endl;  
  out << Qt::endl;
}

LACApp::LACApp(QCoreApplication *app, int argc, char *argv[]) :
  m_app(app)
{
  Q_UNUSED(argc);
  Q_UNUSED(argv);
  QCoreApplication::setApplicationName(QCoreApplication::translate("main", "look-angle-calculator"));
  QCoreApplication::setApplicationVersion(GIT_VERSION);

  QCommandLineParser parser;
  parser.setApplicationDescription("Look Angle Calculator.");
  parser.addHelpOption();
  parser.addVersionOption();

  // A boolean option with multiple names (-i, --interactive)
  QCommandLineOption interactiveOption(QStringList() << "i" << "interactive",
                                       QCoreApplication::translate("main", "Interactive mode to query coordinates."));
  parser.addOption(interactiveOption);

  // Process the actual command line arguments given by the user
  parser.process(*m_app);

  if (parser.isSet("help"))
    help();

  bool isInteractive = parser.isSet(interactiveOption);
  double lat, lon, alt;
  QTextStream err(stderr);
  QTextStream out(stdout);
  QTextStream in(stdin);
  if (isInteractive) {
    out << "Observer's latitude: ";
    out.flush();
    in >> lat;
    out << "Observer's longitude: ";
    out.flush();
    in >> lon;
    out << "Observer's altitude: ";
    out.flush();
    in >> alt;
    m_observer = QGeoCoordinate(lat, lon, alt);
    out << "Target's latitude: ";
    out.flush();
    in >> lat;
    out << "Target's longitude: ";
    out.flush();
    in >> lon;
    out << "Target's altitude: ";
    out.flush();
    in >> alt;
    m_target = QGeoCoordinate(lat, lon, alt);
  } else {
    in >> lat >> lon >> alt;
    m_observer = QGeoCoordinate(lat, lon, alt);
    in >> lat >> lon >> alt;
    m_target = QGeoCoordinate(lat, lon, alt);
  }
  if (!m_observer.isValid()) {
    err << "ERROR: observer - invalid coordinate" << Qt::endl;
    emit finished();
    return;
  }
if (!m_target.isValid()) {
  QTextStream err(stderr);
  err << "ERROR: target - invalid coordinate" << Qt::endl;
  emit finished();
  return;
 }
}

// TODO: get arguments from command line
void LACApp::main()
{
  QTextStream stream(stdout);
  LookAngle lookAngle(m_observer, m_target);
  GeoPoint observer_ecef(m_observer);
  GeoPoint target_ecef(m_target);
  
  stream << "========================" << Qt::endl;
  stream << "   observer's location: " << m_observer.toString() << Qt::endl;
  stream << "   observed's location: " << m_target.toString() << Qt::endl;
  stream << "     azimuth to target: " << lookAngle.azimuth() << Qt::endl;
  stream << "   elevation to target: " << lookAngle.elevation() << Qt::endl;
  stream << "LoS distance to target: " << observer_ecef.distanceTo(target_ecef) << Qt::endl;
  
  emit finished();
}
