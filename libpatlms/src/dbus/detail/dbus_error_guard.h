#ifndef PATLMS_DBUS_DBUS_ERROR_GUARD_H
#define PATLMS_DBUS_DBUS_ERROR_GUARD_H

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

#endif
