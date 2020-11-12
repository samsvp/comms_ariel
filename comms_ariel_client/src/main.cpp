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
#include "../proto/comms_ariel.pb.h"
#include "../include/connect.hpp"
#include "../include/messages.hpp"

using namespace std;

int main(int argc, char const *argv[])
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

  send_message(m, "LOCALHOST", 7000);

  return 0;
}
