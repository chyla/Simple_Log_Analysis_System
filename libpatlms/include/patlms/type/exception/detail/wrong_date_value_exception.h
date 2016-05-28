#ifndef INCLUDE_PATLMS_TYPE_EXCEPTION_DETAIL_EXCEPTION_H
#define INCLUDE_PATLMS_TYPE_EXCEPTION_DETAIL_EXCEPTION_H

#include <patlms/type/exception/exception.h>

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

#endif /* INCLUDE_PATLMS_TYPE_EXCEPTION_DETAIL_EXCEPTION_H */
