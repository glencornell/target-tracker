QT       += core
QT       += positioning
QT       += sensors
QT       += gui

# Application Name:
TARGET = blos-asset

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += src/look-at.hpp \
           src/vector3d.hpp \
           src/Direction.hpp \
           src/GeoEntity.hpp \
           src/GeoTarget.hpp \
           src/GeoObserver.hpp \
           src/RotationReadingSource.hpp \
           src/data-sources/LogFilePositionSource.hpp \
           src/BlosApp.hpp

SOURCES += src/main.cpp \
           src/look-at.cpp \
           src/Direction.cpp \
           src/GeoEntity.cpp \
           src/GeoTarget.cpp \
           src/GeoObserver.cpp \
           src/RotationReadingSource.cpp \
           src/data-sources/LogFilePositionSource.cpp \
           src/BlosApp.cpp

RESOURCES += \
           logfile.qrc
