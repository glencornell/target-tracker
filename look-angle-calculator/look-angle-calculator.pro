include ("../common.pri")

# Application Name:
TARGET     = look-angle-calculator

CONFIG    += console
CONFIG    += debug
CONFIG    -= app_bundle

TEMPLATE   = app

HEADERS   += LACApp.hpp

SOURCES   += main.cpp \
             LACApp.cpp

symbian: LIBS += -lgeotracker
else:unix|win32: LIBS += -L$$OUT_PWD/../src/ -lgeotracker

#INCLUDEPATH += $$PWD/../src
#DEPENDPATH += $$PWD/../src

win32: PRE_TARGETDEPS += $$OUT_PWD/../src/geotracker.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../src/libgeotracker.a

include (../deployment.pri)
include (../gitversion.pri)
