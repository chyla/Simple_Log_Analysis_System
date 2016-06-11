/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBPATLMS_INCLUDE_DBUS_EXCEPTION_DBUS_EXCEPTION_H
#define LIBPATLMS_INCLUDE_DBUS_EXCEPTION_DBUS_EXCEPTION_H

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

#endif /* DBUS_EXCEPTION_H */
