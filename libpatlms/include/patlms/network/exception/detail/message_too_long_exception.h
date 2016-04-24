#ifndef MESSAGE_TOO_LONG_H
#define MESSAGE_TOO_LONG_H

#include <patlms/network/exception/network_exception.h>

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


#endif /* MESSAGE_TOO_LONG_H */

