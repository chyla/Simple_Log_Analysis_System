#pragma once

#include <patlms/type/time.h>
#include <patlms/type/date.h>

namespace type
{

class Timestamp {
 public:
  inline static Timestamp Create(int hour, int minute, int second,
                                 int day, int month, int year);
  inline static Timestamp Create(const Time &time, const Date &date);

  inline void Set(int hour, int minute, int second,
                  int day, int month, int year);

  inline void Set(const Time &time, const Date &date);

  inline const Time& GetTime() const;
  inline const Date& GetDate() const;

  inline const std::string ToString() const;

  inline bool operator==(const Timestamp &t2) const;
  inline bool operator<(const Timestamp &t2) const;

 private:
  Time time_;
  Date date_;
};

Timestamp Timestamp::Create(int hour, int minute, int second,
                            int day, int month, int year) {
  Timestamp t;
  t.Set(hour, minute, second, day, month, year);

  return t;
}

Timestamp Timestamp::Create(const Time &time, const Date &date) {
  Timestamp t;
  t.Set(time, date);

  return t;
}

void Timestamp::Set(int hour, int minute, int second,
                    int day, int month, int year) {
  time_ = Time::Create(hour, minute, second);
  date_ = Date::Create(day, month, year);
}

void Timestamp::Set(const Time &time, const Date &date) {
  time_ = time;
  date_ = date;
}

const Time& Timestamp::GetTime() const {
  return time_;
}

const Date& Timestamp::GetDate() const {
  return date_;
}

const std::string Timestamp::ToString() const {
  return date_.ToString() + " " + time_.ToString();
}

bool Timestamp::operator==(const Timestamp &t2) const {
  return (GetDate() == t2.GetDate()) && (GetTime() == t2.GetTime());
}

bool Timestamp::operator<(const Timestamp &t2) const {
  return ((GetDate() == t2.GetDate()) && (GetTime() < t2.GetTime())) ||
    (GetDate() < t2.GetDate());
}

std::ostream& operator<<(std::ostream& os, const type::Timestamp &timestamp);

}
