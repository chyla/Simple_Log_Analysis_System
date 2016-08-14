#pragma once

#include <slas/type/exception/exception.h>

namespace type
{

namespace exception
{

namespace detail
{

class WrongDateValueException : public interface::Exception {
  inline char const* what() const throw () override;
};

char const* WrongDateValueException::what() const throw () {
  return "Wrong time value.";
}

}

}

}
