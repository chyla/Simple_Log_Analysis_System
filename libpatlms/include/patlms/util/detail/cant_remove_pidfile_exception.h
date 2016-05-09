#ifndef LIBPATLMS_UTIL_DETAIL_CANT_REMOVE_PIDFILE_EXCEPTION_H
#define LIBPATLMS_UTIL_DETAIL_CANT_REMOVE_PIDFILE_EXCEPTION_H

#include <patlms/type/exception/exception.h>

namespace util
{

namespace detail
{

class CantRemovePidfileException : public interface::Exception {
  inline char const* what() const throw () override;
};

char const* CantRemovePidfileException::what() const throw () {
  return "Can't remove pidfile.";
}

}

}

#endif /* LIBPATLMS_UTIL_DETAIL_CANT_REMOVE_PIDFILE_EXCEPTION_H */
