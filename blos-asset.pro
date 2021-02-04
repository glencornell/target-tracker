TEMPLATE  = subdirs

SUBDIRS  += src \
            tests \
            blos-asset

# Define the build-time directory dependencies
tests.depends = src
blos-asset.depends = src

# Common configurations
include ("common.pri")

# Additional files to be included in the distribution
DISTFILES +=
