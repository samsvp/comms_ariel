#pragma once
#include "../proto/comms_ariel.pb.h"

#define MAXDATASIZE 4096  // max number of bytes we can get at once

/* simple little function to write an error string and exit */
void err(const char* s);

void send_message(comms_ariel::DroneToUSVMessage msg, const char* hostname, int port);