/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "send_command.h"

#include <boost/log/trivial.hpp>
#include <patlms/util/configure_logger.h>
#include <patlms/network/network.h>

#include "slas_parser.hpp"

using namespace std;
using namespace network;

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

  NetworkPtr network = Network::Create();
  int socket_fd = -1;

  try {
    socket_fd = network->Socket(PF_UNIX);
    network->ConnectUnix(socket_fd, options->GetBashSocketPath());

    network->SendText(socket_fd, command);

    network->Close(socket_fd);
  }
  catch (exception &ex) {
    BOOST_LOG_TRIVIAL(fatal) << "slas::SendCommand: Exception catched: " << ex.what();
  }
}

}
