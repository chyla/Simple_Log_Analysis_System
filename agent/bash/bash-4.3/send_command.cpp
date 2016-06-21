/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "send_command.h"

#include <cstdio>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#warning Correct bash socket path
#define SOCK_MOD_PATH "/var/run/patlms/bash.socket"
#define MAX_PATH 109

extern "C"
{

void SendCommand(const char *command) {
  int fd, res;
  struct sockaddr_un saddr;

  fd = socket(PF_UNIX, SOCK_STREAM, 0);

  saddr.sun_family = AF_UNIX;
  strcpy(saddr.sun_path, SOCK_MOD_PATH);

  res = connect(fd, (struct sockaddr *) &saddr, sizeof (struct sockaddr_un));

  if (res == 0) {
    send(fd, command, strlen(command) + 1, 0);
    close(fd);
  }
  else {
    fprintf(stderr, "Can't connect to log server.");
  }
}

}
