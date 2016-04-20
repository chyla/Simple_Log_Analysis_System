#ifndef SOCKET_IS_NOT_OPEN_H
#define SOCKET_IS_NOT_OPEN_H

#include "src/bash/exception/bash_exception.h"

namespace bash
{

namespace exception
{

namespace detail
{

class SocketIsNotOpenException : public ::bash::exception::BashException {
 public:
  inline char const* what() const throw ();
};

char const* SocketIsNotOpenException::what() const throw () {
  return "Socket is not open.";
}

}

}

}

#endif /* SOCKET_IS_NOT_OPEN_H */
