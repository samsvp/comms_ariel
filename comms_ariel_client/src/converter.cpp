#include "../include/converter.hpp"

double deg2rad(double angle)
{
    return angle * _rad2deg;
}

tf2::Quaternion enu2nwu(tf2::Quaternion nwu_orientation)
{
    tf2::Quaternion enu_orientation = nwu_orientation * _NWU2ENU;    
    return enu_orientation.normalized();
}