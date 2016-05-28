#include <patlms/type/date.h>
#include <patlms/type/exception/detail/wrong_date_value_exception.h>
#include <patlms/type/exception/detail/date_parse_exception.h>

#include <regex>

namespace type
{

const Date Date::Create(int day, int month, int year) {
  if ((day < 1 || day > 31) ||
      (month < 1 || month > 12) ||
      (year < 1970))
    throw exception::detail::WrongDateValueException();

  return Date(day, month, year);
}

const Date Date::Create(const std::string &date) {
  int day = 0, month = 0, year = 0;
  std::regex pattern("^\\s*([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})\\s*$");

  std::cmatch cm;
  if (std::regex_match(date.c_str(), cm, pattern)) {
    year = stoi(cm[1]);
    month = stoi(cm[2]);
    day = stoi(cm[3]);
  }
  else
    throw exception::detail::DateParseException();

  return Date::Create(day, month, year);
}

const std::string Date::ToString() const {
  auto helper = [](int i) {
    return (i < 10 ? "0" : "") +std::to_string(i);
  };

  const std::string date_string = std::to_string(year_) + "-" + helper(month_) + "-" + helper(day_);

  return date_string;
}

std::ostream& operator<<(std::ostream& os, const type::Date &date) {
  os << date.GetYear() << "-" << date.GetMonth() << "-" << date.GetDay();
  return os;
}

}
