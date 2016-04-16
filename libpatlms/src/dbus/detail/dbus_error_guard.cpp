#include <patlms/dbus/detail/dbus_error_guard.h>


namespace dbus
{

DBusErrorGuard::DBusErrorGuard(DBusError *error)
  : error_(error)
{
  dbus_error_init(error_);
}

DBusErrorGuard::~DBusErrorGuard()
{
  dbus_error_free(error_);
}
  
}
