#include <iostream>
#include "../proto/comms_ariel.pb.h"
#include "../include/connect.hpp"

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
  std::cout << "Common:" << std::endl;
  std::cout << m.common().id() << std::endl;
  std::cout << m.common().time_us() << std::endl;

  std::cout << "telemetry message" << std::endl;
  std::cout << m.telemetry().latitude() << std::endl;
  std::cout << m.telemetry().longitude() << std::endl;
  //std::cout << m.telemetry().orientation() << std::endl;
  std::cout << m.telemetry().velocity_n() << std::endl;
  std::cout << m.telemetry().velocity_w() << std::endl;
  std::cout << m.telemetry().state() << std::endl;

  std::cout << "Mission:" << std::endl;
  std::cout << m.mission().mission_id() << std::endl;
  std::cout << m.mission().usv_reference_time() << std::endl;
}

int main(int argc, char const *argv[])
{
  receive_drone_message(7000, drone_callback);

  return 0;
}
