/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SLAS_OPTIONS_H
#define SLAS_OPTIONS_H

#include <string>

namespace slas
{

class Options {
 public:

  Options(const std::string &bash_socket_path,
          bool debug) :
  bash_socket_path_(bash_socket_path),
  debug_(debug) {
  }

  inline const std::string& GetBashSocketPath() const;
  inline bool IsDebug() const;

 private:
  std::string bash_socket_path_;
  bool debug_;
};

const std::string& Options::GetBashSocketPath() const {
  return bash_socket_path_;
}

bool Options::IsDebug() const {
  return debug_;
}

}

#endif /* SLAS_OPTIONS_H */
