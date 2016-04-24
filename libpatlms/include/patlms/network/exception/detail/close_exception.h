#ifndef CLOSE_EXCEPTION_H
#define CLOSE_EXCEPTION_H

#include <patlms/network/exception/network_exception.h>

namespace network
{

namespace exception
{

namespace detail
{

class CloseException : public ::network::exception::NetworkException {
 public:
  inline char const* what() const throw ();
};

char const* CloseException::what() const throw() {
  return "Can't close socket.";
}

}

}

}

#endif /* CLOSE_EXCEPTION_H */
