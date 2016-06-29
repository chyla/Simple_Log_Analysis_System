/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef INCLUDE_PATLMS_TYPE_WRONG_DAY_OF_WEEK_EXCEPTION_H
#define INCLUDE_PATLMS_TYPE_WRONG_DAY_OF_WEEK_EXCEPTION_H

#include <patlms/type/exception/exception.h>

namespace type
{

namespace exception
{

namespace detail
{

class WrongDayOfWeekException : public interface::Exception {
  inline char const* what() const throw () override;
};

char const* WrongDayOfWeekException::what() const throw () {
  return "Wrong day of week";
}

}

}

}

#endif /* WRONG_DAY_OF_WEEK_EXCEPTION_H */
