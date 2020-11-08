#include <iostream>
#include "proto/comms_ariel.pb.h"
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

#define PORT 7000           // the port users will be connecting to
#define MAXDATASIZE 4096   // max number of bytes we can send at once
#define BACKLOG 10          // how many pending connections queue will hold

using namespace std;
void sigchld_handler(int s) {
  while (waitpid(-1, NULL, WNOHANG) > 0);
}

/* simple little function to write an error string and exit */
static void err(const char* s) {
  perror(s);
  exit(EXIT_FAILURE);
}

void connect(int& listenfd)
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
  server.sin_port = htons(PORT);              // short, network byte order
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

void receive_message()
{
  int listenfd;
  int connectfd;
  int numbytes;
  char buf[MAXDATASIZE];
  socklen_t sin_size;
  struct sockaddr_in client;

  connect(listenfd);

  while (1) {     // main accept() loop
    sin_size = sizeof(struct sockaddr_in);

    connectfd = accept(listenfd, (struct sockaddr *)&client, &sin_size);

    numbytes = recv(connectfd, buf, MAXDATASIZE, 0);
    buf[numbytes] = '\0';
    string a = buf;
    cout << "You got a message from " << inet_ntoa(client.sin_addr) << endl;

    // Receive  msg from clients
    comms_ariel::DroneToUSVMessage m;
    m.ParsePartialFromArray(buf, sizeof(buf) / sizeof(char));
    // Check if everything is ok
    cout << "Common:" << endl;
    cout << m.common().id() << endl;
    cout << m.common().time_us() << endl;

    cout << "telemetry message" << endl;
    cout << m.telemetry().loaded_mission_id() << endl;
    cout << m.telemetry().latitude() << endl;
    cout << m.telemetry().longitude() << endl;
    cout << m.telemetry().altitude() << endl;
    cout << m.telemetry().state() << endl;

    close(connectfd);
  }

  close(listenfd);
}

int main(int argc, char const *argv[])
{
  receive_message();

  return 0;
}
