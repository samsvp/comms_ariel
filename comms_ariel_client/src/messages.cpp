#include "../include/messages.hpp"

void fill_common_data(comms_ariel::CommonTelemetry* msg, uint64_t time_us, uint32_t id)
{
  msg->set_id(id);
  msg->set_time_us(time_us);
}

void fill_mission_data(comms_ariel::Mission* msg, uint64_t mission_id, std::vector<double> waypoints,
                       uint64_t usv_reference_time, std::vector<double> usv_waypoints)
{
  //if (waypoints.size() != 4 || usv_waypoints.size() != 4) throw;
  
  msg->set_mission_id(mission_id);
  for(int i=0; i<3; i++) msg->add_waypoints(waypoints[i]);
  msg->set_usv_reference_time(usv_reference_time);
  for(int i=0; i<3; i++) msg->add_usv_waypoints(usv_waypoints[i]);
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

void fill_usv_data(comms_ariel::USVTelemetry* msg, double latitude, double longitude, 
                   std::vector<double> orientation, float velocity_n, float velocity_w,
                   comms_ariel::USVTelemetry::States state)
{
  //if (orientation.size() != 4) throw;

  msg->set_latitude(latitude);
  msg->set_longitude(longitude);
  for(int i=0; i < 4; i++) 
  {
    msg->add_orientation(orientation[i]);
    //msg->set_orientation(i, orientation[i]);
  }
  msg->set_velocity_n(velocity_n);
  msg->set_velocity_w(velocity_w);
  msg->set_state(state);  
}