#pragma once

#include <slas/network/exception/network_exception.h>

namespace network
{

namespace exception
{

namespace detail
{

class MessageTooLongException : public ::network::exception::NetworkException {
 public:
  inline char const* what() const throw ();
};

char const* MessageTooLongException::what() const throw () {
  return "Message too long.";
}

}

}

}
