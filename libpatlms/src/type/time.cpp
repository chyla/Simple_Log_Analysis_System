#include <patlms/type/time.h>

#include <patlms/type/exception/detail/wrong_time_value_exception.h>
#include <patlms/type/exception/detail/time_parse_exception.h>

#include <regex>

namespace type
{

const Time Time::Create(int hour, int minute, int second) {
  Time t;
  t.Set(hour, minute, second);

  return t;
}

const Time Time::Create(const std::string &time) {
  int hour = 0, minute = 0, second = 0;
  std::regex pattern("^\\s*([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})\\s*$");

  std::cmatch cm;
  if (std::regex_match(time.c_str(), cm, pattern)) {
    hour = stoi(cm[1]);
    minute = stoi(cm[2]);
    second = stoi(cm[3]);
  }
  else
    throw exception::detail::TimeParseException();

  return Time::Create(hour, minute, second);
}

void Time::Set(int hour, int minute, int second) {
  CheckTime(hour, minute, second);

  hour_ = hour;
  minute_ = minute;
  second_ = second;
}

const std::string Time::ToString() const {
  auto helper = [](int i) {
    return (i < 10 ? "0" : "") +std::to_string(i);
  };

  const std::string time_string = helper(hour_) + ":" + helper(minute_) + ":" + helper(second_);

  return time_string;
}

bool Time::operator<(const Time &t2) const {
  if ((GetHour() == t2.GetHour()) &&
      (GetMinute() == t2.GetMinute()) &&
      (GetSecond() < t2.GetSecond()))
    return true;
  else if ((GetHour() == t2.GetHour()) &&
      (GetMinute() < t2.GetMinute()))
    return true;
  else if ((GetHour() < t2.GetHour()))
    return true;

  return false;
}

void Time::CheckTime(int hour, int minute, int second) {
  if ((hour < 0 || hour > 24) ||
      (minute < 0 || minute > 59) ||
      (second < 0 || second > 59)) {
    throw exception::detail::WrongTimeValueException();
  }
}

std::ostream& operator<<(std::ostream& os, const type::Time &time) {
  os << time.GetHour() << ":" << time.GetMinute() << ":" << time.GetSecond();
  return os;
}

}
