#include <iostream>
#include <vector>
#include <algorithm>
#include "../proto/comms_ariel.pb.h"
#include "../include/connect.hpp"
#include "ros/ros.h"
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/Vector3Stamped.h"
#include "geometry_msgs/QuaternionStamped.h"


ros::Publisher position_pub;
ros::Publisher velocity_pub;
ros::Publisher orientation_pub;

/*
 * Receives the drone message and outputs it to a ros topic
 */
void drone_callback(comms_ariel::DroneToUSVMessage& m)
{
  std::cout << "Common:" << std::endl;
  std::cout << m.common().id() << std::endl;
  std::cout << m.common().time_us() << std::endl;

  std::cout << "telemetry message" << std::endl;
  std::cout << m.telemetry().loaded_mission_id() << std::endl;
  std::cout << m.telemetry().latitude() << std::endl;
  std::cout << m.telemetry().longitude() << std::endl;
  std::cout << m.telemetry().altitude() << std::endl;
  std::cout << m.telemetry().state() << std::endl;
}

void usv_callback(comms_ariel::USVToDroneMessage& m)
{
  ros::Time time_now = ros::Time::now();
  // Position
  geometry_msgs::PointStamped point;
  point.point.x = m.telemetry().longitude();
  point.point.y = m.telemetry().latitude();
  point.point.z = 0;
  point.header.stamp = time_now;
  position_pub.publish(point);

  // Velocity
  geometry_msgs::Vector3Stamped vector3;
  vector3.vector.x = m.telemetry().velocity_w();
  vector3.vector.y = m.telemetry().velocity_n();
  vector3.vector.z = 0;
  vector3.header.stamp = time_now;
  velocity_pub.publish(vector3);

  // Orientation
  geometry_msgs::QuaternionStamped quaternion;
  std::vector<double> p_data;
  std::copy(m.telemetry().orientation().begin(), m.telemetry().orientation().end(),
            std::back_inserter(p_data));

  quaternion.quaternion.x = p_data[0];
  quaternion.quaternion.y = p_data[1];
  quaternion.quaternion.z = p_data[2];
  quaternion.quaternion.w = p_data[3];
  quaternion.header.stamp = time_now;
  orientation_pub.publish(quaternion);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "usv_data");
  ros::NodeHandle n;
  position_pub = n.advertise<geometry_msgs::PointStamped>("usv_position", 1000);
  velocity_pub = n.advertise<geometry_msgs::Vector3Stamped>("usv_velocity", 1000);
  orientation_pub = n.advertise<geometry_msgs::QuaternionStamped>("usv_orientation", 1000);
  ros::Rate loop_rate(10);
  
  receive_message(6000, usv_callback, ros::ok);

  return 0;
}
