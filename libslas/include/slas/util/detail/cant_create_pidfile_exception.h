#pragma once

#include <slas/type/exception/exception.h>

namespace util
{

namespace detail
{

class CantCreatePidfileException : public interface::Exception {
  inline char const* what() const throw () override;
};

char const* CantCreatePidfileException::what() const throw () {
  return "Can't create pidfile.";
}

}

}
