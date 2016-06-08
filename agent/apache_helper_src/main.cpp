#include <iostream>
#include <string>

#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
#include <patlms/network/network.h>
#include <patlms/util/configure_logger.h>
#include <patlms/type/exception/exception.h>

#include "program_options/parse_options.h"

using namespace program_options;
using namespace network;
using namespace std;

int main(int argc, char **argv) {
  Options options;
  try {
    options = ParseOptions(argc, argv);
  }
  catch (interface::Exception &ex) {
    cerr << "Wrong option: " << ex.what() << '\n';
    return 1;
  }
  catch (exception &ex) {
    cerr << "Missing program option (requied --socket and --logfile).\n";
    return 1;
  }

  util::ConfigureLogger(options.GetLogFilePath());

  NetworkPtr network = Network::Create();
  int socket_fd = -1;

  try {
    string log_line;
    while (getline(cin, log_line)) {
      socket_fd = network->Socket(PF_UNIX);
      network->ConnectUnix(socket_fd, options.GetSocketPath());

      network->SendText(socket_fd, log_line);

      network->Close(socket_fd);
    }
  }
  catch (exception &ex) {
    BOOST_LOG_TRIVIAL(fatal) << "Exception catched: " << ex.what();
  }

  return 0;
}
