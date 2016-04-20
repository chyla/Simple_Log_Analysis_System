#ifndef LOOP_ACCEPT_EXCEPTION_H
#define LOOP_ACCEPT_EXCEPTION_H

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

#endif /* LOOP_ACCEPT_EXCEPTION_H */
