#include <gtest/gtest.h>

#include <patlms/type/exception/detail/time_parse_exception.h>
#include "patlms/type/time.h"

using namespace testing;
using namespace std;
using namespace type;

TEST(Time, FromString_CorrectDate) {
  string date = "2016-05-18";

  Time t = Time::FromString(date);

  ASSERT_EQ(18, t.GetDay());
  ASSERT_EQ(5, t.GetMonth());
  ASSERT_EQ(2016, t.GetYear());
  ASSERT_EQ(0, t.GetHour());
  ASSERT_EQ(0, t.GetMinute());
  ASSERT_EQ(0, t.GetSecond());
}

TEST(Time, FromString_CorrectDateWithOneDigitInMonthAndDay) {
  string date = "2016-5-8";

  Time t = Time::FromString(date);

  ASSERT_EQ(8, t.GetDay());
  ASSERT_EQ(5, t.GetMonth());
  ASSERT_EQ(2016, t.GetYear());
  ASSERT_EQ(0, t.GetHour());
  ASSERT_EQ(0, t.GetMinute());
  ASSERT_EQ(0, t.GetSecond());
}


TEST(Time, FromString_CorrectDateWithTime) {
  string date = "2016-05-18 10:02:33";

  Time t = Time::FromString(date);

  ASSERT_EQ(18, t.GetDay());
  ASSERT_EQ(5, t.GetMonth());
  ASSERT_EQ(2016, t.GetYear());
  ASSERT_EQ(10, t.GetHour());
  ASSERT_EQ(2, t.GetMinute());
  ASSERT_EQ(33, t.GetSecond());
}

TEST(Time, FromString_CorrectDateWithTimeWithOneDigitInTime) {
  string date = "2016-05-18 9:2:3";

  Time t = Time::FromString(date);

  ASSERT_EQ(18, t.GetDay());
  ASSERT_EQ(5, t.GetMonth());
  ASSERT_EQ(2016, t.GetYear());
  ASSERT_EQ(9, t.GetHour());
  ASSERT_EQ(2, t.GetMinute());
  ASSERT_EQ(3, t.GetSecond());
}

TEST(Time, FromString_CorrectDateWithTimeAndOneDigitInMonthAndDay) {
  string date = "2016-5-8 10:02:33";

  Time t = Time::FromString(date);

  ASSERT_EQ(8, t.GetDay());
  ASSERT_EQ(5, t.GetMonth());
  ASSERT_EQ(2016, t.GetYear());
  ASSERT_EQ(10, t.GetHour());
  ASSERT_EQ(2, t.GetMinute());
  ASSERT_EQ(33, t.GetSecond());
}

TEST(Time, FromString_WrongDateWithAdditionalChar) {
  string date = "2016-5-8s";

  ASSERT_THROW(Time::FromString(date), ::type::exception::detail::TimeParseException);
}

TEST(Time, FromString_WrongDateAndTimeWithAdditionalChar) {
  string date = "2016-5-8 10:02:33s";

  ASSERT_THROW(Time::FromString(date), ::type::exception::detail::TimeParseException);
}
