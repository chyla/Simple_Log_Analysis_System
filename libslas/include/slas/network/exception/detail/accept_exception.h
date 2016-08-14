#pragma once

#include <slas/network/exception/network_exception.h>

namespace network
{

namespace exception
{

namespace detail
{

class AcceptException : public ::network::exception::NetworkException {
 public:
  inline char const* what() const throw ();
};

char const* AcceptException::what() const throw() {
  return "Accept exception.";
}

}

}

}
