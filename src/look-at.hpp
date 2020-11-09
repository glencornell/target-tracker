#pragma once

#include <QGeoCoordinate>

// calculate the look angle of point b from the frame of reference of
// point a.  Points a & b are geocentric coordinates (including
// altitude in meters above the geoid surface).  This function returns
// the straight line distance (in meters), the azimuth (in degrees
// from true north) and elevation angle (in degrees from the horizon)
// at point a to point b.
void lookAt(QGeoCoordinate const &a,
            QGeoCoordinate const &b,
            const bool oblate,
            double *los_distance,
            double *azimuth,
            double *elevation);

