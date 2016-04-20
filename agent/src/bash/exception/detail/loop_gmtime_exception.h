#ifndef LOOP_GMTIME_EXCEPTION_H
#define LOOP_GMTIME_EXCEPTION_H

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

#endif /* LOOP_GMTIME_EXCEPTION_H */
