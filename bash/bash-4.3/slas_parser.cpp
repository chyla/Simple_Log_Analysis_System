/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "slas_parser.hpp"

#include <sstream>

using namespace std;
using namespace boost::program_options;


namespace slas
{

Parser::Parser()
: options_("Options") {
  options_.add_options()
      ("logfile", value<string>(), "logfile path")
      ("bash_socket_path", value<string>(), "Bash socket path")
      ("enable-debug", value<bool>(), "change log-level to debug")
      ;
}

void Parser::SetConfigFilePath(const string& path) {
  config_file_path_ = path;
}

const Options Parser::Parse() {
  typedef boost::program_options::variables_map variables_map;
  variables_map variables;

  const char *config_file = config_file_path_.c_str();
  store(parse_config_file<char>(config_file, options_), variables);

  notify(variables);

  return Options(variables["logfile"].as<string>(),
                 variables["bash_socket_path"].as<string>(),
                 variables["enable-debug"].as<bool>());
}

}
