QT       += core
QT       += positioning
QT       -= gui

# Application Name:
TARGET = blos-asset

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += src/look-at.hpp \
           src/vector3d.hpp

SOURCES += src/main.cpp \
           src/look-at.cpp
