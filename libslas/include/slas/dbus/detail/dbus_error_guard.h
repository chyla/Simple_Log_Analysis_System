#pragma once

#include "dbus_interface.h"

namespace dbus
{

namespace detail
{

class DBusErrorGuard
{
 public:
  DBusErrorGuard(DBusInterfacePtr dbus_interface, DBusError *error);
  ~DBusErrorGuard();

 private:
  DBusInterfacePtr dbus_interface_;
  DBusError *error_;
};

}

}
