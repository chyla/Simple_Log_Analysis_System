#pragma once

#include "src/bash/exception/bash_exception.h"

namespace bash
{

namespace exception
{

namespace detail
{

class LoopGMTimeException : public ::bash::exception::BashException {
 public:
  inline char const* what() const throw ();
};

char const* LoopGMTimeException::what() const throw () {
  return "GMTime failed.";
}

}

}

}
