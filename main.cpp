#include <iostream>
#include "proto/comms_ariel.pb.h"
#include "boost/asio.hpp"

using namespace boost::asio;

using namespace std;

void send_message(const char* msg, int msg_size, const char* IP, const char* port)
{
  io_service io_service;
  ip::udp::socket socket(io_service);
  ip::udp::endpoint remote_endpoint;

  socket.open(ip::udp::v4());

  remote_endpoint = ip::udp::endpoint(ip::address::from_string("192.168.0.4"), 9000);

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
  comms_ariel::DroneToUSVMessage m;
  fill_drone_data(m.mutable_telemetry(), 1, 22.0, 42.0, 10.f,
     comms_ariel::DroneTelemetry::States::DroneTelemetry_States_WAITING_LANDING);
  fill_common_data(m.mutable_common(), 1, 1);
  string s;
  m.SerializeToString(&s);
  const char *msg = s.c_str();
  send_message(msg, s.length() - 1, "192.168.0.0", "7000");
  /*
  cout << "Hello Proto!" << endl;
  
  cout << "telemetry message" << endl;
  cout << m.telemetry().loaded_mission_id() << endl;
  cout << m.telemetry().latitude() << endl;
  cout << m.telemetry().longitude() << endl;
  cout << m.telemetry().altitude() << endl;
  cout << m.telemetry().state() << endl;

  cout << "common message" << endl;
  cout << m.common().id() << endl;
  cout << m.common().time_us() << endl;
  */
  return 0;
}
