#ifndef WRONG_PATH_EXCEPTION_H
#define WRONG_PATH_EXCEPTION_H

#include <patlms/type/exception/exception.h>

namespace type
{

namespace exception
{

namespace detail
{

class WrongPathException : public interface::Exception {
  inline char const* what() const throw () override;
};

char const* WrongPathException::what() const throw () {
  return "Wrong file path.";
}

}

}

}

#endif /* WRONG_PATH_EXCEPTION_H */
