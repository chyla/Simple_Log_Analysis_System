#include <patlms/type/date.h>
#include <patlms/type/exception/detail/wrong_date_value_exception.h>
#include <patlms/type/exception/detail/date_parse_exception.h>

#include <regex>
#include <array>

using namespace std;

namespace type
{

const Date Date::Create(int day, int month, int year) {
  Date d;
  d.Set(day, month, year);

  return d;
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

void Date::Set(int day, int month, int year) {
  CheckDate(day, month, year);

  day_ = day;
  month_ = month;
  year_ = year;
}

const std::string Date::ToString() const {
  auto helper = [](int i) {
    return (i < 10 ? "0" : "") +std::to_string(i);
  };

  const std::string date_string = std::to_string(year_) + "-" + helper(month_) + "-" + helper(day_);

  return date_string;
}

Date Date::GetYesterdayDate() const {
  Date yesterday;

  if (GetDay() > 1) {
    yesterday.Set(GetDay() - 1, GetMonth(), GetYear());
  }
  else if ((GetDay() == 1) && (GetMonth() > 1)) {
    yesterday.Set(GetNumberOfDayInMonth(GetMonth() - 1),
                  GetMonth() - 1,
                  GetYear());
  }
  else if ((GetDay() == 1) && (GetMonth() == 1)) {
    yesterday.Set(GetNumberOfDayInMonth(12),
                  12,
                  GetYear() - 1);
  }

  return yesterday;
}

bool Date::operator==(const Date &t2) const {
  return (t2.GetDay() == GetDay()) &&
      (t2.GetMonth() == GetMonth()) &&
      (t2.GetYear() == GetYear());
}

bool Date::operator<(const Date &t2) const {
  if ((GetYear() == t2.GetYear()) &&
      (GetMonth() == t2.GetMonth()) &&
      (GetDay() < t2.GetDay()))
    return true;
  else if ((GetYear() == t2.GetYear()) &&
      (GetMonth() < t2.GetMonth()))
    return true;
  else if ((GetYear() < t2.GetYear()))
    return true;

  return false;
}

void Date::CheckDate(int day, int month, int year) {
  if ((day < 1 || day > 31) ||
      (month < 1 || month > 12) ||
      (year < 1970))
    throw exception::detail::WrongDateValueException();
}

const int Date::GetNumberOfDayInMonth(int month) const {
  array<int, 12> day_of_months = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  if (((GetYear() % 4 == 0) && (GetYear() % 100 != 0)) ||
      (GetYear() % 400 == 0)) {
    day_of_months.at(1) = 29;
  }

  return day_of_months.at(month - 1);
}

std::ostream& operator<<(std::ostream& os, const type::Date &date) {
  os << date.GetYear() << "-" << date.GetMonth() << "-" << date.GetDay();
  return os;
}

}
