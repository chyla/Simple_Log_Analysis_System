#ifndef LOOP_RECV_EXCEPTION_H
#define LOOP_RECV_EXCEPTION_H

#include "src/bash/exception/bash_exception.h"

namespace bash
{

namespace exception
{

namespace detail
{

class LoopRecvException : public ::bash::exception::BashException {
 public:
  inline char const* what() const throw ();
};

char const* LoopRecvException::what() const throw () {
  return "Recv failed.";
}

}

}

}

#endif /* LOOP_RECV_EXCEPTION_H */
