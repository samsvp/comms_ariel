comms_ariel
======================

Ariel's USV and Drone communication package.

Dependencies
======================

Download Protobuf from https://github.com/protocolbuffers/protobuf/releases, and extract the file. Then go to the Protobuf directory you just extracted and run:


    $ ./configure
    $ make
    $ make check
    $ sudo make install
    $ sudo ldconfig

Install
======================
    $ catkin_make

Usage
======================

To receive a drone message:
```cpp
#include "../proto/comms_ariel.pb.h"
#include "../include/connect.hpp"

void drone_callback(comms_ariel::DroneToUSVMessage& m)
{
    /* code */
}

int main(int argc, char const *argv[])
{
  int port = 7000; //port where the message will be received
  receive_drone_message(7000, drone_callback);
  return 0;
}
```
To send a drone message:
```cpp
#include "../proto/comms_ariel.pb.h"
#include "../include/connect.hpp"
#include "../include/messages.hpp"

int main(int argc, char const *argv[])
{
  comms_ariel::DroneToUSVMessage m;
  fill_drone_data(m.mutable_telemetry(), 1, 22.0, 42.0, 10.f,
     comms_ariel::DroneTelemetry::States::DroneTelemetry_States_WAITING_LANDING);
  fill_common_data(m.mutable_common(), 1, 1);
  send_message(m, "192.168.0.1", 7000); // Server IP and port
  return 0;
}
```
To receive a USV message:
```cpp
#include "../proto/comms_ariel.pb.h"
#include "../include/connect.hpp"

void usv_callback(comms_ariel::USVToDroneMessage& m)
{
    /* code */
}

int main(int argc, char const *argv[])
{
  int port = 7000; //port where the message will be received
  receive_drone_message(7000, usv_callback);
  return 0;
}
```
To send a USV message:
```cpp
#include "../proto/comms_ariel.pb.h"
#include "../include/connect.hpp"
#include "../include/messages.hpp"

int main(int argc, char const *argv[])
{
  comms_ariel::USVToDroneMessage m;
  double usv_waypoints[3] = {22.0, 42.0, 5.0};
  double waypoints[3] = {22.0, 42.0, 1};
  fill_usv_data(m.mutable_telemetry(), 22.0, 42.0, 10.0, 10.f, 10.f
     comms_ariel::USVTelemetry::States::DroneTelemetry_States_TRANSIT);
  fill_common_data(m.mutable_common(), 1, 1);
  fill_mission_data(m.mutable_mission(), 1,waypoints , 1, usv_waypoints);
  send_message(m, "192.168.0.1", 7000); // Server IP and port
  return 0;
}
```
Test
======================

To test if everything is ok, initialize the server:

    $ build/comms_ariel_server/main_server
    
Then initialize the server:

    $ build/comms_ariel_client/main_client
    
If everything is fine you should see the same message printed on the client and server terminal's.
