#ifndef CANT_OPEN_SOCKET_H
#define CANT_OPEN_SOCKET_H

#include "src/bash/exception/bash_exception.h"

namespace bash
{

namespace exception
{

namespace detail
{

class CantOpenSocketException : public ::bash::exception::BashException {
 public:
  inline char const* what() const throw ();
};

char const* CantOpenSocketException::what() const throw() {
  return "Can't open socket.";
}

}

}

}

#endif /* CANT_OPEN_SOCKET_H */
