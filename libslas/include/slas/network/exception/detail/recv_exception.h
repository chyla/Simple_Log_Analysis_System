#pragma once

#include <slas/network/exception/network_exception.h>

namespace network
{

namespace exception
{

namespace detail
{

class RecvException : public ::network::exception::NetworkException {
 public:
  inline char const* what() const throw ();
};

char const* RecvException::what() const throw() {
  return "Recv function error.";
}

}

}

}
