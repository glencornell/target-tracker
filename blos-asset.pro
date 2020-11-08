QT       += core
QT       += positioning
QT       -= gui

# Application Name:
TARGET = blos-asset

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += look-at.hpp

SOURCES += main.cpp \
           look-at.cpp

