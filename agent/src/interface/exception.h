#pragma once

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
