#ifndef LIBPATLMS_DBUS_DETAIL_DBUS_ERROR_GUARD_H
#define LIBPATLMS_DBUS_DETAIL_DBUS_ERROR_GUARD_H

#include <dbus/dbus.h>


namespace dbus
{

class DBusErrorGuard
{
 public:
  DBusErrorGuard(DBusError *error);
  ~DBusErrorGuard();

 private:
  DBusError *error_;
};

}

#endif // LIBPATLMS_DBUS_DETAIL_DBUS_ERROR_GUARD_H
