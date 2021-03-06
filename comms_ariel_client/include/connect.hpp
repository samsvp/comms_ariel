#pragma once
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

#define MAXDATASIZE 4096  // max number of bytes we can get at once

/* simple little function to write an error string and exit */
void err(const char* s);

template<class T>
void send_message(T msg, const char* hostname, int port)
{
  int fd;
  struct hostent *he;
  struct sockaddr_in server;

  if ((he = gethostbyname(hostname)) == NULL) {
      err("gethostbyname");
  }

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      err("socket");
  }

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr = *((struct in_addr *)he->h_addr);

  if (connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
      err("connect");
  }
  std::string s;
  msg.SerializeToString(&s);
  void *buffer = malloc(s.size());
  msg.SerializeToArray(buffer, s.size());
  send(fd, buffer, s.size(), 0);
  free(buffer);

  close(fd);
  std::cout << "Message sent" << std::endl;
}
