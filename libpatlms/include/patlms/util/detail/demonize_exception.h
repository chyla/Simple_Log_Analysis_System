#pragma once

#include <patlms/type/exception/exception.h>

namespace util
{

namespace detail
{

class DemonizeException : public interface::Exception {
  inline char const* what() const throw () override;
};

char const* DemonizeException::what() const throw () {
  return "Can't demonize.";
}

}

}
