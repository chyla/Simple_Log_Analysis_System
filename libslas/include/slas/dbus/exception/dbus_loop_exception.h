#pragma once

#include <slas/type/exception/exception.h>

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
