#ifndef LIBPATLMS_DBUS_DETAIL_DBUS_ERROR_GUARD_H
#define LIBPATLMS_DBUS_DETAIL_DBUS_ERROR_GUARD_H

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

#endif // LIBPATLMS_DBUS_DETAIL_DBUS_ERROR_GUARD_H
