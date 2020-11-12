#pragma once

#define PORT 7000           // the port client will be connecting to
#define MAXDATASIZE 4096  // max number of bytes we can get at once
#define BACKLOG 10          // how many pending connections queue will hold

/* simple little function to write an error string and exit */
void err(const char* s);
void sigchld_handler(int s);
void connect(int& listenfd, int port);
void receive_message(char* buf, int listenfd);
void receive_drone_message(int port, void (*callback)(comms_ariel::DroneToUSVMessage&));
void receive_usv_message(int port, void (*callback)(comms_ariel::USVToDroneMessage&));