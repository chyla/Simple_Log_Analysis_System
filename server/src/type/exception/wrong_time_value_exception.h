#ifndef TIME_EXCEPTION_H
#define TIME_EXCEPTION_H

#include "src/interface/exception.h"

namespace type
{

namespace exception
{

class WrongTimeValueException : public interface::Exception {
  inline char const* what() const throw () override;
};

char const* WrongTimeValueException::what() const throw ()
{
  return "Wrong time value.";
}

}

}

#endif /* TIME_EXCEPTION_H */
