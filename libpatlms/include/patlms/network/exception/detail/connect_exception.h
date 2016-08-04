#pragma once

#include <patlms/network/exception/network_exception.h>

namespace network
{

namespace exception
{

namespace detail
{

class ConnectException : public ::network::exception::NetworkException {
 public:
  inline char const* what() const throw ();
};

char const* ConnectException::what() const throw () {
  return "Connect exception.";
}

}

}

}
