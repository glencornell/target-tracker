TEMPLATE  = subdirs

SUBDIRS  += src \
            tests \
            target-tracker \
            look-angle-calculator

# Define the build-time directory dependencies
tests.depends = src
target-tracker.depends = src
look-angle-calculator.depends = src

# Common configurations
include ("common.pri")

# Additional files to be included in the distribution
DISTFILES +=
