#include <patlms/type/time.h>

#include <patlms/type/exception/detail/wrong_time_value_exception.h>

namespace type
{

void Time::Set(int hour, int minute, int second,
               int day, int month, int year) {
  if ((hour < 0 || hour > 59) ||
    (minute < 0 || minute > 59) ||
    (second < 0 || second > 59) ||
    (day < 1 || day > 31) ||
    (month < 1 || month > 12) ||
    (year < 1970)) {
    throw exception::detail::WrongTimeValueException();
  }

  hour_ = hour;
  minute_ = minute;
  second_ = second;
  day_ = day;
  month_ = month;
  year_ = year;
}

std::ostream& operator<<(std::ostream& os, const type::Time &time) {
  os << time.GetYear() << "-" << time.GetMonth() << "-" << time.GetDay() << " "
    << time.GetHour() << ":" << time.GetMinute() << ":" << time.GetSecond();
  return os;
}

}
