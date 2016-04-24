#ifndef BAD_ADDRESS_H
#define BAD_ADDRESS_H

#include <patlms/network/exception/network_exception.h>

namespace network
{

namespace exception
{

namespace detail
{

class BadAddressException : public ::network::exception::NetworkException {
 public:
  inline char const* what() const throw ();
};

char const* BadAddressException::what() const throw() {
  return "Bad address.";
}

}

}

}

#endif /* BAD_ADDRESS_H */
