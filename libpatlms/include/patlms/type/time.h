#ifndef PATLMS_TYPE_TIME_H
#define PATLMS_TYPE_TIME_H

#include <ostream>

namespace type
{

class Time {
 public:
  inline Time();

  void Set(int hour, int minute, int second,
           int day, int month, int year);

  static const Time Create(int hour, int minute, int second,
                           int day, int month, int year);
  static const Time FromString(const std::string &time);

  const std::string ToString() const;

  inline int GetMinute() const;
  inline int GetHour() const;
  inline int GetSecond() const;

  inline int GetDay() const;
  inline int GetMonth() const;
  inline int GetYear() const;

 private:
  int second_, minute_, hour_, day_, month_, year_;
};

Time::Time()
: second_(0),
minute_(0),
hour_(0),
day_(0),
month_(0),
year_(0) {
}

int Time::GetMinute() const {
  return minute_;
}

int Time::GetHour() const {
  return hour_;
}

int Time::GetSecond() const {
  return second_;
}

int Time::GetDay() const {
  return day_;
}

int Time::GetMonth() const {
  return month_;
}

int Time::GetYear() const {
  return year_;
}


std::ostream& operator<<(std::ostream& os, const type::Time &time);

}

#endif /* PATLMS_TYPE_TIME_H */
