#ifndef PATLMS_INTERFACE_EXCEPTION_H
#define PATLMS_INTERFACE_EXCEPTION_H

#include <exception>

namespace interface
{

class Exception : public std::exception {
 public:
  virtual inline ~Exception();

  virtual char const* what() const throw () = 0;
};

Exception::~Exception() {
}

}

#endif /* PATLMS_INTERFACE_EXCEPTION_H */

