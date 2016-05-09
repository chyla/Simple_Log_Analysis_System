#ifndef LIBPATLMS_UTIL_DETAIL_CANT_REMOVE_FILE_EXCEPTION_H
#define LIBPATLMS_UTIL_DETAIL_CANT_REMOVE_FILE_EXCEPTION_H

#include <patlms/type/exception/exception.h>

namespace util
{

namespace detail
{

class CantRemoveFileException : public interface::Exception {
  inline char const* what() const throw () override;
};

char const* CantRemoveFileException::what() const throw () {
  return "Can't remove file.";
}

}

}

#endif /* LIBPATLMS_UTIL_DETAIL_CANT_REMOVE_FILE_EXCEPTION_H */
