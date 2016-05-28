#ifndef INCLUDE_PATLMS_EXCEPTION_DETAIL_DATE_PARSE_EXCEPTION_H
#define INCLUDE_PATLMS_EXCEPTION_DETAIL_DATE_PARSE_EXCEPTION_H

#include <patlms/type/exception/exception.h>

namespace type
{

namespace exception
{

namespace detail
{

class DateParseException : public interface::Exception {
  inline char const* what() const throw () override;
};

char const* DateParseException::what() const throw () {
  return "Can't parse date.";
}

}

}

}

#endif /* INCLUDE_PATLMS_EXCEPTION_DETAIL_DATE_PARSE_EXCEPTION_H */
