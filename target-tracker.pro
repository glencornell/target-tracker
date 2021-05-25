TEMPLATE  = subdirs

SUBDIRS  += libgeotracker \
            tests \
            target-tracker \
            look-angle-calculator

# Define the build-time directory dependencies
tests.depends = libgeotracker
target-tracker.depends = libgeotracker
look-angle-calculator.depends = libgeotracker

# Common configurations
include ("common.pri")

# Additional files to be included in the distribution
DISTFILES +=
