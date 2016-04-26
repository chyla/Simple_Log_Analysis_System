#ifndef CONNECT_EXCEPTION_H
#define CONNECT_EXCEPTION_H

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


#endif /* CONNECT_EXCEPTION_H */
