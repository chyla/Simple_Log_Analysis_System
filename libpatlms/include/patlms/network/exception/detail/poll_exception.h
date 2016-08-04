#pragma once

#include <patlms/network/exception/network_exception.h>

namespace network
{

namespace exception
{

namespace detail
{

class PollException : public ::network::exception::NetworkException {
 public:
  inline char const* what() const throw ();
};

char const* PollException::what() const throw () {
  return "Poll exception.";
}

}

}

}
