#include <iostream>
#include "proto/comms_ariel.pb.h"
#include "boost/asio.hpp"

using namespace boost::asio;

using namespace std;

void send_message(const char* msg, int msg_size, const char* IP, int port)
{
  io_service io_service;
  ip::udp::socket socket(io_service);
  ip::udp::endpoint remote_endpoint;

  socket.open(ip::udp::v4());

  remote_endpoint = ip::udp::endpoint(ip::address::from_string(IP), port);

  boost::system::error_code err;
  socket.send_to(buffer(msg, msg_size), remote_endpoint, 0, err);

  socket.close();
}

void fill_common_data(comms_ariel::CommonTelemetry* msg, uint64_t time_us, uint32_t id)
{
  msg->set_id(id);
  msg->set_time_us(time_us);
}

void fill_drone_data(comms_ariel::DroneTelemetry* msg, uint64_t loaded_mission_id,
                     double latitude, double longitude, float altitude, 
                     comms_ariel::DroneTelemetry::States state)
{
  msg->set_loaded_mission_id(loaded_mission_id);
  msg->set_latitude(latitude);
  msg->set_longitude(longitude);
  msg->set_altitude(altitude);
  msg->set_state(state);  
}

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

  string s;
  m.SerializeToString(&s);
  char *msg = new char[s.size() + 1];
  copy(s.begin(), s.end(), msg);
  msg[s.size()] = '\0';
  string s2;
  int size_arr = sizeof(msg) / sizeof(char); 
  s2.assign(msg, size_arr);

  void *buffer = malloc(s.size());
  m.SerializeToArray(buffer, s.size());

  cout << "\nchar with string " << (string(msg) == s2) << "\n" << endl;
  cout << "\nstring with string " << (s == s2) << "\n" << endl;
  comms_ariel::DroneToUSVMessage m2;
  
  m2.ParsePartialFromArray(buffer, s.size());
  cout << "Common:" << endl;
  cout << m2.common().id() << endl;
  cout << m2.common().time_us() << endl;

  cout << "telemetry message" << endl;
  cout << m2.telemetry().loaded_mission_id() << endl;
  cout << m2.telemetry().latitude() << endl;
  cout << m2.telemetry().longitude() << endl;
  cout << m2.telemetry().altitude() << endl;
  cout << m2.telemetry().state() << endl;
  send_message(msg, s.length() - 1, "192.168.0.151", 7000);
  
  cout << "Message sent" << endl;

  return 0;
}
