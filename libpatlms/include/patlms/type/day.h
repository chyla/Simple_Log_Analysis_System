/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "exception/detail/wrong_day_of_week_exception.h"

namespace type
{

enum class Day {
  SUNDAY,
  MONDAY,
  TUESDAY,
  WEDNESDAY,
  THURSDAY,
  FRIDAY,
  SATURDAY
};

inline Day ToDay(int day_of_week) {
  switch (day_of_week)
  {
  case 0:
    return Day::SUNDAY;

  case 1:
    return Day::MONDAY;

  case 2:
    return Day::TUESDAY;

  case 3:
    return Day::WEDNESDAY;

  case 4:
    return Day::THURSDAY;

  case 5:
    return Day::FRIDAY;

  case 6:
    return Day::SATURDAY;

  default:
    throw exception::detail::WrongDayOfWeekException();
  }
}

inline const std::string ToString(const Day &day) {
  switch (day)
  {
  case Day::SUNDAY:
    return "Sun";

  case Day::MONDAY:
    return "Mon";

  case Day::TUESDAY:
    return "Tue";

  case Day::WEDNESDAY:
    return "Wed";

  case Day::THURSDAY:
    return "Thu";

  case Day::FRIDAY:
    return "Fri";

  case Day::SATURDAY:
    return "Sat";
  }
}

}
