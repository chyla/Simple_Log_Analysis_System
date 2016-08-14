#pragma once

#include <slas/network/exception/network_exception.h>

namespace network
{

namespace exception
{

namespace detail
{

class CantOpenSocketException : public ::network::exception::NetworkException {
 public:
  inline char const* what() const throw ();
};

char const* CantOpenSocketException::what() const throw() {
  return "Can't open socket.";
}

}

}

}
