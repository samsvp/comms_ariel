#include <unistd.h>
#include <iostream>
#include "proto/comms_ariel.pb.h"
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>

#define HOSTNAME "LOCALHOST"
#define PORT 7000           // the port client will be connecting to
#define MAXDATASIZE 4096  // max number of bytes we can get at once

using namespace std;

/* simple little function to write an error string and exit */
static void err(const char* s) {
    perror(s);
    exit(EXIT_FAILURE);
}

void send_message(comms_ariel::DroneToUSVMessage msg)
{
  int fd;
  struct hostent *he;
  struct sockaddr_in server;

  if ((he = gethostbyname(HOSTNAME)) == NULL) {
      err("gethostbyname");
  }

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      err("socket");
  }

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr = *((struct in_addr *)he->h_addr);

  if (connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
      err("connect");
  }
  string s;
  msg.SerializeToString(&s);
  void *buffer = malloc(s.size());
  msg.SerializeToArray(buffer, s.size());
  send(fd, buffer, s.size(), 0);

  close(fd);
  cout << "Message sent" << endl;
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

  send_message(m);

  return 0;
}
