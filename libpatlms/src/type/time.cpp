#include <patlms/type/time.h>

#include <patlms/type/exception/detail/wrong_time_value_exception.h>
#include <patlms/type/exception/detail/time_parse_exception.h>

#include <regex>

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

const Time Time::Create(int hour, int minute, int second,
                        int day, int month, int year) {
  Time t;
  t.Set(hour, minute, second, day, month, year);
  return t;
}

const Time Time::FromString(const std::string &time) {
  int hour = 0, minute = 0, second = 0, day = 0, month = 0, year = 0;
  std::regex pattern("^([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})$");
  std::regex pattern_with_time("^([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})\\s+([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})$");

  std::cmatch cm;
  if (std::regex_match(time.c_str(), cm, pattern)) {
    year = stoi(cm[1]);
    month = stoi(cm[2]);
    day = stoi(cm[3]);
  }
  else if (std::regex_match(time.c_str(), cm, pattern_with_time)) {
    year = stoi(cm[1]);
    month = stoi(cm[2]);
    day = stoi(cm[3]);
    hour = stoi(cm[4]);
    minute = stoi(cm[5]);
    second = stoi(cm[6]);
  }
  else
    throw exception::detail::TimeParseException();

  return Time::Create(hour, minute, second, day, month, year);
}

const std::string Time::ToString() const {
  auto helper = [](int i) {
    return (i < 10 ? "0" : "") +std::to_string(i);
  };

  const std::string time_string = std::to_string(year_) + "-" + helper(month_) + "-" + helper(day_)
      + " " + helper(hour_) + ":" + helper(minute_) + ":" + helper(second_);

  return time_string;
}

std::ostream& operator<<(std::ostream& os, const type::Time &time) {
  os << time.GetYear() << "-" << time.GetMonth() << "-" << time.GetDay() << " "
      << time.GetHour() << ":" << time.GetMinute() << ":" << time.GetSecond();
  return os;
}

}
