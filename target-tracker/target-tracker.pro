include ("../common.pri")

# Application Name:
TARGET     = target-tracker

CONFIG    += console
CONFIG    += no_testcase_installs
CONFIG    -= app_bundle

TEMPLATE   = app

HEADERS   += TargetTrackerApp.hpp

SOURCES   += main.cpp \
             TargetTrackerApp.cpp

RESOURCES += logfile.qrc

symbian: LIBS += -lgeotracker
else:unix|win32: LIBS += -L$$OUT_PWD/../libgeotracker -lgeotracker

#INCLUDEPATH += $$PWD/../libgeotracker
#DEPENDPATH += $$PWD/../libgeotracker

win32: PRE_TARGETDEPS += $$OUT_PWD/../libgeotracker/geotracker.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../libgeotracker/libgeotracker.a

include (../deployment.pri)
include (../gitversion.pri)
