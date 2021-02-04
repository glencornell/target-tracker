include ("../common.pri")

# Application Name:
TARGET     = blos-asset

CONFIG    += console
CONFIG    += no_testcase_installs
CONFIG    -= app_bundle

TEMPLATE   = app

HEADERS   += BlosApp.hpp

SOURCES   += main.cpp \
             BlosApp.cpp

RESOURCES += logfile.qrc

symbian: LIBS += -lgeotracker
else:unix|win32: LIBS += -L$$OUT_PWD/../src/ -lgeotracker

#INCLUDEPATH += $$PWD/../src
#DEPENDPATH += $$PWD/../src

win32: PRE_TARGETDEPS += $$OUT_PWD/../src/geotracker.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../src/libgeotracker.a

include (deployment.pri)
include (gitversion.pri)
