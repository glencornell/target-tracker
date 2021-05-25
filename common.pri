# This tells all projects that include this file to also use the
# `libgeotracker` project folder as an includes path resolving library
# headers.
INCLUDEPATH += $$PWD/libgeotracker
SRC_DIR = $$PWD

# Every project will need these
QT       += core
QT       += positioning
QT       += sensors
QT       += gui

CONFIG   += debug

# Add additional compiler checks
contains(DEFINES, DEVELOPER_MODE) {
    CONFIG += debug
    *-g++*|*clang* {
        QMAKE_CXXFLAGS += -Werror -Wall -Wextra
    }
    *clang* {
        QMAKE_CXXFLAGS += -Wno-error=inconsistent-missing-override
    }
}
