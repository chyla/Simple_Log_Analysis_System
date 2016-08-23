/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SLAS_PARSER_H
#define SLAS_PARSER_H

#include <boost/program_options.hpp>
#include <utility>
#include <string>

#include "slas_options.hpp"


namespace slas
{

class Parser {
 public:
  Parser();

  void SetConfigFilePath(const std::string &path);

  const Options Parse();

 private:
  typedef boost::program_options::options_description options_description;

  std::string config_file_path_;

  options_description options_;
};

}

#endif 
