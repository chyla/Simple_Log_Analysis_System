#ifndef TIME_PARSE_EXCEPTION_H
#define TIME_PARSE_EXCEPTION_H

#include <patlms/type/exception/exception.h>

namespace type
{

namespace exception
{

namespace detail
{

class TimeParseException : public interface::Exception {
  inline char const* what() const throw () override;
};

char const* TimeParseException::what() const throw () {
  return "Can't parse time.";
}

}

}

}

#endif /* TIME_PARSE_EXCEPTION_H */
