#pragma once

#include "src/bash/exception/bash_exception.h"

namespace bash
{

namespace exception
{

namespace detail
{

class LoopAcceptException : public ::bash::exception::BashException {
 public:
  inline char const* what() const throw ();
};

char const* LoopAcceptException::what() const throw () {
  return "Acccept failed.";
}

}

}

}
