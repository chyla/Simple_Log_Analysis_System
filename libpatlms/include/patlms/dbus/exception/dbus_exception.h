/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include <patlms/type/exception/exception.h>

namespace dbus
{

namespace exception
{

class DBusException : public ::interface::Exception {
 public:
  virtual ~DBusException() = default;

  char const* what() const throw () {
    return "DBus exception";
  }
};

}

}
