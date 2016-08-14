#pragma once

#include <slas/type/exception/exception.h>

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
