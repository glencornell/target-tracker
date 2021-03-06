#include <QtMath>
#include "LookAngle.hpp"
#include "test_LookAngle.hpp"

void test_LookAngle::test_accessors() {
  LookAngle a;

  a.setAzimuth(90.0);
  QVERIFY2(a.azimuth() == 90.0, "setAzimuth");

  a.setElevation(90.0);
  QVERIFY2(a.elevation() == 90.0, "setElevation");
}

void test_LookAngle::test_setLookAngle() {
  QGeoCoordinate observer(39.0, -75.0, 4000.0);
  QGeoCoordinate target(39.0, -76.0, 12000.0);
  LookAngle a;

  a.setLookAngle(observer, target);
  QVERIFY2(qFabs(a.azimuth() - 270.339)  <= 0.001, "azimuth");
  QVERIFY2(qFabs(a.elevation() - 4.8812) <= 0.001, "elevation");
}

// generate basic main: no GUI, no events
QTEST_APPLESS_MAIN(test_LookAngle)
