#pragma once
#include "../proto/comms_ariel.pb.h"
#include <sys/types.h>

void fill_common_data(comms_ariel::CommonTelemetry* msg, uint64_t time_us, uint32_t id);

void fill_mission_data(uint64_t mission_id, double* waypoints,
                       uint64_t usv_reference_time, double* usv_waypoints);

void fill_drone_data(comms_ariel::DroneTelemetry* msg, uint64_t loaded_mission_id,
                     double latitude, double longitude, float altitude, 
                     comms_ariel::DroneTelemetry::States state);

void fill_usv_data(comms_ariel::USVTelemetry* msg, double latitude, double longitude, 
                   double orientation, float velocity_n, float velocity_w,
                   comms_ariel::USVTelemetry::States state);