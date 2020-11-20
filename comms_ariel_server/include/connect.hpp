#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <string>
#include "../proto/comms_ariel.pb.h"


#define PORT 6000           // the port client will be connecting to
#define MAXDATASIZE 4096  // max number of bytes we can get at once
#define BACKLOG 10          // how many pending connections queue will hold

/* simple little function to write an error string and exit */
void err(const char* s);
void sigchld_handler(int s);
void connect(int& listenfd, int port);
void receive_message(char* buf, int listenfd);

template<class T>
void receive_message(int port, void (*callback)(T&), bool (*condition)())
{
  int listenfd;
  connect(listenfd, port);
  char buf[MAXDATASIZE];

  while (condition()) {     // main accept() loop
    receive_message(buf, listenfd);

    T m;
    m.ParsePartialFromArray(buf, sizeof(buf) / sizeof(char));
    callback(m);
  }

  close(listenfd);
}
