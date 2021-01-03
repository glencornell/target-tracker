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
           src/GeoObserver.hpp \
           src/Direction.hpp \
           src/Target.hpp \
           src/RotationReadingSource.hpp \
           src/Asset.hpp \
           src/data-sources/LogFilePositionSource.hpp \
           src/Gimbal.hpp

SOURCES += src/main.cpp \
           src/look-at.cpp \
           src/GeoObserver.cpp \
           src/Direction.cpp \
           src/Target.cpp \
           src/RotationReadingSource.cpp \
           src/Asset.cpp \
           src/data-sources/LogFilePositionSource.cpp \
           src/Gimbal.cpp

RESOURCES += \
           logfile.qrc
