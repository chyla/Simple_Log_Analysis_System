#pragma once

#include <patlms/type/exception/exception.h>

namespace type
{

namespace exception
{

namespace detail
{

class WrongTimeValueException : public interface::Exception {
  inline char const* what() const throw () override;
};

char const* WrongTimeValueException::what() const throw () {
  return "Wrong time value.";
}

}

}

}
