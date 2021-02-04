QT       += core
QT       += positioning
QT       += sensors
QT       += gui

# Application Name:
TARGET = blos-asset

CONFIG   += debug
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += src/GeoPoint.hpp \
           src/LookAngle.hpp \
           src/GeoEntity.hpp \
           src/GeoObserver.hpp \
           src/RotationReadingSource.hpp \
           src/data-sources/LogFilePositionSource.hpp \
           src/BlosApp.hpp

SOURCES += src/main.cpp \
           src/GeoPoint.cpp \
           src/LookAngle.cpp \
           src/GeoEntity.cpp \
           src/GeoObserver.cpp \
           src/RotationReadingSource.cpp \
           src/data-sources/LogFilePositionSource.cpp \
           src/BlosApp.cpp

RESOURCES += \
           logfile.qrc
