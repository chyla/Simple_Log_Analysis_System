#ifndef RECV_EXCEPTION_H
#define RECV_EXCEPTION_H

#include <patlms/network/exception/network_exception.h>

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

#endif /* RECV_EXCEPTION_H */
