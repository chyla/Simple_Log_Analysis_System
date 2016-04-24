#ifndef TIMEOUT_EXCEPTION_H
#define TIMEOUT_EXCEPTION_H

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


#endif /* TIMEOUT_EXCEPTION_H */

