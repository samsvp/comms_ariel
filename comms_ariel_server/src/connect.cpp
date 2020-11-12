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
#include "../include/connect.hpp"


void err(const char* s) {
  perror(s);
  exit(EXIT_FAILURE);
}

void sigchld_handler(int s) {
  while (waitpid(-1, NULL, WNOHANG) > 0);
}

void connect(int& listenfd, int port)
{
  struct sockaddr_in server;
  struct sigaction sa;

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    err("socket");
  }

  int opt = SO_REUSEADDR;
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
    err("setsockopt");
  }

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;                // host byte order
  server.sin_port = htons(port);              // short, network byte order
  server.sin_addr.s_addr = htonl(INADDR_ANY); // automatically fill with my IP


  if (bind(listenfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
    err("bind");
  }

  if (listen(listenfd, BACKLOG) == -1) {
    err("listen");
  }

  sa.sa_handler = sigchld_handler;  // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    err("sigaction");
  }
}

void receive_message(char* buf, int listenfd)
{
  int connectfd;
  int numbytes;
  socklen_t sin_size;
  struct sockaddr_in client;

  sin_size = sizeof(struct sockaddr_in);
  connectfd = accept(listenfd, (struct sockaddr *)&client, &sin_size);
  numbytes = recv(connectfd, buf, MAXDATASIZE, 0);
  buf[numbytes] = '\0';
  std::cout << "You got a message from " << inet_ntoa(client.sin_addr) << std::endl;

  close(connectfd);  
}

void receive_drone_message(int port, void (*callback)(comms_ariel::DroneToUSVMessage&))
{
  int listenfd;
  connect(listenfd, port);
  char buf[MAXDATASIZE];

  while(1) // main accept() loop
  {
    receive_message(buf, listenfd);
    // Receive  msg from clients
    comms_ariel::DroneToUSVMessage m;
    m.ParsePartialFromArray(buf, sizeof(buf) / sizeof(char));
    callback(m);
  }

  close(listenfd);
}

void receive_usv_message(int port, void (*callback)(comms_ariel::USVToDroneMessage&))
{
  int listenfd;
  connect(listenfd, port);
  char buf[MAXDATASIZE];

  while (1) {     // main accept() loop
    receive_message(buf, listenfd);

    comms_ariel::USVToDroneMessage m;
    m.ParsePartialFromArray(buf, sizeof(buf) / sizeof(char));
    callback(m);
  }

  close(listenfd);
}
