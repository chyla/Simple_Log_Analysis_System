#ifndef LIBPATLMS_SRC_DBUS_DETAIL_DBUS_LOOP_EXCEPTION_H
#define LIBPATLMS_SRC_DBUS_DETAIL_DBUS_LOOP_EXCEPTION_H

#include <patlms/type/exception/exception.h>

namespace dbus
{

namespace exception
{

class DBusLoopException : public interface::Exception {
  inline char const* what() const throw () override;
};

char const* DBusLoopException::what() const throw () {
  return "D-Bus loop failed.";
}

}

}

#endif /* LIBPATLMS_SRC_DBUS_DETAIL_DBUS_LOOP_EXCEPTION_H */
