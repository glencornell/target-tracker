include ("../common.pri")
include ("../src/includes.pri")

QT        += testlib

CONFIG    += testcase
CONFIG    += no_testcase_installs

HEADERS   += test_LookAngle.hpp

SOURCES   += test_LookAngle.cpp

# Include dependencies if required
LIBS +=
