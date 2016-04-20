#ifndef LOOP_GETSOCKOPT_EXCEPTION_H
#define LOOP_GETSOCKOPT_EXCEPTION_H

#include "src/bash/exception/bash_exception.h"

namespace bash
{

namespace exception
{

namespace detail
{

class LoopGetsockoptException : public ::bash::exception::BashException {
 public:
  inline char const* what() const throw ();
};

char const* LoopGetsockoptException::what() const throw () {
  return "Getsockopt failed.";
}

}

}

}

#endif /* LOOP_GETSOCKOPT_EXCEPTION_H */
