#ifndef INCLUDE_PATLMS_TYPE_DATE_H
#define INCLUDE_PATLMS_TYPE_DATE_H

#include <string>

namespace type
{

class Date {
 public:
  static const Date Create(int day, int month, int year);
  static const Date Create(const std::string &time);

  const std::string ToString() const;

  inline int GetDay() const;
  inline int GetMonth() const;
  inline int GetYear() const;

 private:
  inline Date(int day, int month, int year);
  
  static void CheckDate(int day, int month, int year);

  int day_, month_, year_;
};

Date::Date(int day, int month, int year)
: day_(day),
month_(month),
year_(year) {
}

int Date::GetDay() const {
  return day_;
}

int Date::GetMonth() const {
  return month_;
}

int Date::GetYear() const {
  return year_;
}

std::ostream& operator<<(std::ostream& os, const type::Date &date);

}

#endif /* INCLUDE_PATLMS_TYPE_DATE_H */
