#pragma once
#include <tf2/LinearMath/Quaternion.h>


const double _rad2deg = 57.2957795131; // 180/pi
const tf2::Quaternion _NWU2ENU = {0, 0, 0.7071068, 0.7071068};

/// Converts an angle from radians to degrees.
double rad2deg(double angle);

/// Converts a quaternion in the NWU orientation to ENU
tf2::Quaternion nwu2enu(tf2::Quaternion nwu_orientation);