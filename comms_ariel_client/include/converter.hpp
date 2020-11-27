#pragma once
#include <tf2/LinearMath/Quaternion.h>


const double _rad2deg = 0.01745329251; // pi/180
/// 3D Rotation (x, y, z)
///  0  1  0
/// -1  0  0
///  0  0  1
const tf2::Quaternion _NWU2ENU = {0, 0, -0.7071068, 0.7071068};

/// Converts an angle from radians to degrees.
double deg2rad(double angle);

/// Converts a quaternion in the NWU orientation to ENU
tf2::Quaternion enu2nwu(tf2::Quaternion nwu_orientation);