#include "LookAngle.hpp"
#include "test_LookAngle.hpp"

void test_LookAngle::test_accessors() {
  LookAngle a;

  a.setAzimuth(90.0);
  QVERIFY2(a.azimuth() == 90.0, "setAzimuth");

  a.setElevation(90.0);
  QVERIFY2(a.elevation() == 90.0, "setElevation");
}

// generate basic main: no GUI, no events
QTEST_APPLESS_MAIN(test_LookAngle)
