#include <gtest/gtest.h>

#include <patlms/type/time.h>
#include <patlms/type/exception/detail/time_parse_exception.h>
#include <patlms/type/exception/detail/wrong_time_value_exception.h>

using namespace testing;
using namespace std;
using namespace type;

TEST(Time, FromString_CorrectTime) {
  string time = "10:02:33";

  Time t = Time::Create(time);

  ASSERT_EQ(10, t.GetHour());
  ASSERT_EQ(2, t.GetMinute());
  ASSERT_EQ(33, t.GetSecond());
}

TEST(Time, FromString_CorrectTimeWithOneDigitInTime) {
  string time = "9:2:3";

  Time t = Time::Create(time);

  ASSERT_EQ(9, t.GetHour());
  ASSERT_EQ(2, t.GetMinute());
  ASSERT_EQ(3, t.GetSecond());
}

TEST(Time, FromString_WhenHourIs25) {
  string time = "25:02:33";

  ASSERT_THROW(Time::Create(time), ::type::exception::detail::WrongTimeValueException);
}

TEST(Time, FromString_WhenMinuteIs60) {
  string time = "24:60:33";

  ASSERT_THROW(Time::Create(time), ::type::exception::detail::WrongTimeValueException);
}

TEST(Time, FromString_WhenSecondIs60) {
  string time = "24:0:60";

  ASSERT_THROW(Time::Create(time), ::type::exception::detail::WrongTimeValueException);
}

TEST(Time, ToString) {
  Time t = Time::Create(9, 3, 1);

  ASSERT_STREQ("09:03:01", t.ToString().c_str());
}

TEST(Time, Create) {
  Time t = Time::Create(9, 2, 3);

  ASSERT_EQ(9, t.GetHour());
  ASSERT_EQ(2, t.GetMinute());
  ASSERT_EQ(3, t.GetSecond());
}

TEST(Time, Create_WhenHourIs0) {
  Time t = Time::Create(0, 2, 3);

  ASSERT_EQ(0, t.GetHour());
  ASSERT_EQ(2, t.GetMinute());
  ASSERT_EQ(3, t.GetSecond());
}

TEST(Time, Create_WhenMinuteIs0) {
  Time t = Time::Create(9, 0, 3);

  ASSERT_EQ(9, t.GetHour());
  ASSERT_EQ(0, t.GetMinute());
  ASSERT_EQ(3, t.GetSecond());
}

TEST(Time, Create_WhenSecondIs0) {
  Time t = Time::Create(9, 2, 0);

  ASSERT_EQ(9, t.GetHour());
  ASSERT_EQ(2, t.GetMinute());
  ASSERT_EQ(0, t.GetSecond());
}

TEST(Time, Create_WhenHourIs25) {
  ASSERT_THROW(Time::Create(25, 2, 3), ::type::exception::detail::WrongTimeValueException);
}

TEST(Time, Create_WhenMinuteIs60) {
  ASSERT_THROW(Time::Create(9, 60, 3), ::type::exception::detail::WrongTimeValueException);
}

TEST(Time, Create_WhenSecondIs60) {
  ASSERT_THROW(Time::Create(9, 2, 60), ::type::exception::detail::WrongTimeValueException);
}

TEST(Time, Set) {
  Time t;
  t.Set(9, 2, 3);

  ASSERT_EQ(9, t.GetHour());
  ASSERT_EQ(2, t.GetMinute());
  ASSERT_EQ(3, t.GetSecond());
}

TEST(Time, Set_WhenHourIs0) {
  Time t;
  t.Set(0, 2, 3);

  ASSERT_EQ(0, t.GetHour());
  ASSERT_EQ(2, t.GetMinute());
  ASSERT_EQ(3, t.GetSecond());
}

TEST(Time, Set_WhenMinuteIs0) {
  Time t;
  t.Set(9, 0, 3);

  ASSERT_EQ(9, t.GetHour());
  ASSERT_EQ(0, t.GetMinute());
  ASSERT_EQ(3, t.GetSecond());
}

TEST(Time, Set_WhenSecondIs0) {
  Time t;
  t.Set(9, 2, 0);

  ASSERT_EQ(9, t.GetHour());
  ASSERT_EQ(2, t.GetMinute());
  ASSERT_EQ(0, t.GetSecond());
}

TEST(Time, Set_WhenHourIs25) {
  Time t;

  ASSERT_THROW(t.Set(25, 2, 3), ::type::exception::detail::WrongTimeValueException);
}

TEST(Time, Set_WhenMinuteIs60) {
  Time t;

  ASSERT_THROW(t.Set(9, 60, 3), ::type::exception::detail::WrongTimeValueException);
}

TEST(Time, Set_WhenSecondIs60) {
  Time t;

  ASSERT_THROW(t.Set(9, 2, 60), ::type::exception::detail::WrongTimeValueException);
}

TEST(Time, OperatorLess0) {
  Time t1 = Time::Create(16, 2, 1);
  Time t2 = Time::Create(16, 2, 1);

  EXPECT_FALSE(t1 < t2);
}

TEST(Time, OperatorLess1) {
  Time t1 = Time::Create(16, 2, 1);
  Time t2 = Time::Create(17, 2, 1);

  EXPECT_TRUE(t1 < t2);
}

TEST(Time, OperatorLess2) {
  Time t1 = Time::Create(16, 2, 1);
  Time t2 = Time::Create(16, 3, 1);

  EXPECT_TRUE(t1 < t2);
}

TEST(Time, OperatorLess3) {
  Time t1 = Time::Create(16, 2, 1);
  Time t2 = Time::Create(16, 2, 2);

  EXPECT_TRUE(t1 < t2);
}

TEST(Time, OperatorLess4) {
  Time t1 = Time::Create(16, 30, 1);
  Time t2 = Time::Create(17, 2, 2);

  EXPECT_TRUE(t1 < t2);
}

TEST(Time, OperatorLess5) {
  Time t1 = Time::Create(16, 30, 30);
  Time t2 = Time::Create(17, 2, 2);

  EXPECT_TRUE(t1 < t2);
}

TEST(Time, OperatorLess6) {
  Time t1 = Time::Create(17, 3, 4);
  Time t2 = Time::Create(16, 2, 2);

  EXPECT_FALSE(t1 < t2);
}
