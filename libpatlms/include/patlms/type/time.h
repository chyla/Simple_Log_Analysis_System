#ifndef INCLUDE_PATLMS_TYPE_TIME_H
#define INCLUDE_PATLMS_TYPE_TIME_H

#include <ostream>
#include <string>

namespace type
{

class Time {
 public:
  inline Time();
  Time(const Time &) = default;

  static const Time Create(int hour, int minute, int second);
  static const Time Create(const std::string &time);

  void Set(int hour, int minute, int second);

  const std::string ToString() const;

  inline int GetMinute() const;
  inline int GetHour() const;
  inline int GetSecond() const;

  bool operator==(const Time &t2) const;
  bool operator<(const Time &t2) const;
  Time operator-(long seconds) const;
  Time operator+(long seconds) const;

 private:
  static void CheckTime(int hour, int minute, int second);

  int second_, minute_, hour_;
};

Time::Time() :
second_(0),
minute_(0),
hour_(0) {
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

std::ostream& operator<<(std::ostream& os, const type::Time &time);

}

#endif /* INCLUDE_PATLMS_TYPE_TIME_H */
