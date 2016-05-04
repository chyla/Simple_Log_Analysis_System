#ifndef LIBPATLMS_UTIL_DETAIL_DEMONIZE_EXCEPTION_H
#define LIBPATLMS_UTIL_DETAIL_DEMONIZE_EXCEPTION_H

#include <patlms/type/exception/exception.h>

namespace util
{

namespace detail
{

class DemonizeException : public interface::Exception {
  inline char const* what() const throw () override;
};

char const* DemonizeException::what() const throw () {
  return "Can't demonize.";
}

}

}

#endif /* LIBPATLMS_UTIL_DETAIL_DEMONIZE_EXCEPTION_H */
