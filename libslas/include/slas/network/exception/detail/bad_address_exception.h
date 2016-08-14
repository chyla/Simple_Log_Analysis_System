#pragma once

#include <slas/network/exception/network_exception.h>

namespace network
{

namespace exception
{

namespace detail
{

class BadAddressException : public ::network::exception::NetworkException {
 public:
  inline char const* what() const throw ();
};

char const* BadAddressException::what() const throw() {
  return "Bad address.";
}

}

}

}
