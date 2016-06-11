#include <patlms/dbus/detail/dbus_error_guard.h>


namespace dbus
{

namespace detail
{

DBusErrorGuard::DBusErrorGuard(DBusInterfacePtr dbus_interface, DBusError *error) :
dbus_interface_(dbus_interface),
error_(error) {
  dbus_interface_->error_init(error_);
}

DBusErrorGuard::~DBusErrorGuard() {
  dbus_interface_->error_free(error_);
}

}

}
