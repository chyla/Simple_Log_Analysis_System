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

#include <patlms/util/configure_logger.h>

#include "slas_parser.hpp"

extern "C"
{

void SendCommand(const char *command) {
  static const slas::Options *options = nullptr;
  if (options == nullptr) {
    slas::Parser p;
    p.SetConfigFilePath(BASH_SLAS_CONF_PATH);
    options = new slas::Options(p.Parse());
  }

  util::ConfigureLogger(options->GetLogfilePath(), options->IsDebug());

  int fd, res;
  struct sockaddr_un saddr;

  fd = socket(PF_UNIX, SOCK_STREAM, 0);

  saddr.sun_family = AF_UNIX;
  strcpy(saddr.sun_path, options->GetBashSocketPath().c_str());

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
