#ifndef LIBPATLMS_UTIL_DETAIL_CANT_CREATE_PIDFILE_EXCEPTION_H
#define LIBPATLMS_UTIL_DETAIL_CANT_CREATE_PIDFILE_EXCEPTION_H

#include <patlms/type/exception/exception.h>

namespace util
{

namespace detail
{

class CantCreatePidfileException : public interface::Exception {
  inline char const* what() const throw () override;
};

char const* CantCreatePidfileException::what() const throw () {
  return "Can't create pidfile.";
}

}

}

#endif /* LIBPATLMS_UTIL_DETAIL_CANT_CREATE_PIDFILE_EXCEPTION_H */
