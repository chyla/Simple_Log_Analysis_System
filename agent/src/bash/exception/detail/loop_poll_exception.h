#pragma once

#include "src/bash/exception/bash_exception.h"

namespace bash
{

namespace exception
{

namespace detail
{

class LoopPollException : public ::bash::exception::BashException {
 public:
  inline char const* what() const throw ();
};

char const* LoopPollException::what() const throw () {
  return "Poll failed.";
}

}

}

}
