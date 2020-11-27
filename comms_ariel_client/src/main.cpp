#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "../proto/comms_ariel.pb.h"
#include "../include/connect.hpp"
#include "../include/messages.hpp"

using namespace std;

/*
 * Simulates the messages sent by the drone
 */
void send_drone_mock()
{
  cout << "Filling message" << endl;

  comms_ariel::DroneToUSVMessage m;
  
  fill_drone_data(m.mutable_telemetry(), 1, 22.0, 42.0, 10.f,
     comms_ariel::DroneTelemetry::States::DroneTelemetry_States_WAITING_LANDING);
  fill_common_data(m.mutable_common(), 1, 1);
  cout << "Sending the following message:" << endl;
  
  cout << "telemetry message" << endl;
  cout << m.telemetry().loaded_mission_id() << endl;
  cout << m.telemetry().latitude() << endl;
  cout << m.telemetry().longitude() << endl;
  cout << m.telemetry().altitude() << endl;
  cout << m.telemetry().state() << endl;

  cout << "common message" << endl;
  cout << m.common().id() << endl;
  cout << m.common().time_us() << endl;

  send_message(m, "LOCALHOST", 6000);
}

/*
 * Simulates the messages sent by the usv
 */
void send_usv_mock()
{
  uint64_t m_time = 0;
  
  while(1)
  {
    cout << "Filling message" << endl;
    comms_ariel::USVToDroneMessage m;
    double r = ((double) rand() / (RAND_MAX));
    fill_usv_data(m.mutable_telemetry(), 3.14 / 2 * r, 3.14  * r, {0.0, 0.0, 0.0, 1.0},
                  20.0f * r, 15.0f * r, comms_ariel::USVTelemetry_States_LANDING_OK);
    fill_mission_data(m.mutable_mission(), 0, {1.0, 0.0, 2.0, 3.0},
                      m_time, {1.0, 0.0, 2.0, 3.0});
    fill_common_data(m.mutable_common(), m_time++, 1);

    cout << "Sending the following message:" << endl;
    
    std::cout << "Common:" << std::endl;
    std::cout << m.common().id() << std::endl;
    std::cout << m.common().time_us() << std::endl;

    std::cout << "telemetry message" << std::endl;
    std::cout << m.telemetry().latitude() << std::endl;
    std::cout << m.telemetry().longitude() << std::endl;
    for (const double& o: m.telemetry().orientation())
    {
      std::cout << o;
    }
    std::cout << m.telemetry().velocity_n() << std::endl;
    std::cout << m.telemetry().velocity_w() << std::endl;
    std::cout << m.telemetry().state() << std::endl;

    std::cout << "Mission:" << std::endl;
    std::cout << m.mission().mission_id() << std::endl;
    std::cout << m.mission().usv_reference_time() << std::endl;

    send_message(m, "LOCALHOST", 6000);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main(int argc, char const *argv[])
{
  send_usv_mock();

  return 0;
}
