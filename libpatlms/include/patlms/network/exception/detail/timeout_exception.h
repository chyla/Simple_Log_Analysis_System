#pragma once

#include <patlms/network/exception/network_exception.h>

namespace network
{

namespace exception
{

namespace detail
{

class TimeoutException : public ::network::exception::NetworkException {
 public:
  inline char const* what() const throw ();
};

char const* TimeoutException::what() const throw() {
  return "Timeout exceeded.";
}

}

}

}
