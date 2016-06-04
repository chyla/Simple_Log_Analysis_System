#include <gtest/gtest.h>

#include <patlms/type/date.h>
#include <patlms/type/exception/detail/date_parse_exception.h>
#include <patlms/type/exception/detail/wrong_date_value_exception.h>

using namespace testing;
using namespace std;
using namespace type;

TEST(Date, Create_FromString_WhenCorrectDate) {
  string date = "2016-05-18";

  Date t = Date::Create(date);

  ASSERT_EQ(18, t.GetDay());
  ASSERT_EQ(5, t.GetMonth());
  ASSERT_EQ(2016, t.GetYear());
}

TEST(Date, Create_FromString_WhenCorrectDateWithOneDigitInMonthAndDay) {
  string date = "2016-5-8";

  Date t = Date::Create(date);

  ASSERT_EQ(8, t.GetDay());
  ASSERT_EQ(5, t.GetMonth());
  ASSERT_EQ(2016, t.GetYear());
}

TEST(Date, Create_FromString_WhenCorrectDateWithAdditionalSpacesAtTheBegining) {
  string date = "  2016-5-8";

  Date t = Date::Create(date);

  ASSERT_EQ(8, t.GetDay());
  ASSERT_EQ(5, t.GetMonth());
  ASSERT_EQ(2016, t.GetYear());
}

TEST(Date, Create_FromString_WhenCorrectDateWithAdditionalSpacesAtTheEnd) {
  string date = "2016-5-8     ";

  Date t = Date::Create(date);

  ASSERT_EQ(8, t.GetDay());
  ASSERT_EQ(5, t.GetMonth());
  ASSERT_EQ(2016, t.GetYear());
}

TEST(Date, Create_FromString_WhenWrongDateWithAdditionalChar) {
  string date = "2016-5-8s";

  ASSERT_THROW(Date::Create(date), ::type::exception::detail::DateParseException);
}

TEST(Date, Create_FromString_WhenDayIs0) {
  string date = "2016-5-0";

  ASSERT_THROW(Date::Create(date), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, Create_FromString_WhenMonthIs0) {
  string date = "2016-0-1";

  ASSERT_THROW(Date::Create(date), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, Create_FromString_WhenDayIs32) {
  string date = "2016-5-32";

  ASSERT_THROW(Date::Create(date), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, Create_FromString_WhenMonthIs13) {
  string date = "2016-13-1";

  ASSERT_THROW(Date::Create(date), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, Create_FromString_WhenYearIs1969) {
  string date = "1969-5-1";

  ASSERT_THROW(Date::Create(date), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, Create_FromString_WhenYearContainsThreeDigits) {
  string date = "170-5-1";

  ASSERT_THROW(Date::Create(date), ::type::exception::detail::DateParseException);
}

TEST(Date, Create_FromString_WhenYearContainsTwoDigits) {
  string date = "70-5-1";

  ASSERT_THROW(Date::Create(date), ::type::exception::detail::DateParseException);
}

TEST(Date, Create_FromString_WhenYearContainsOneDigit) {
  string date = "1-5-1";

  ASSERT_THROW(Date::Create(date), ::type::exception::detail::DateParseException);
}

TEST(Date, Create_CorrectDate) {
  Date t;

  t.Set(8, 5, 2016);

  ASSERT_EQ(8, t.GetDay());
  ASSERT_EQ(5, t.GetMonth());
  ASSERT_EQ(2016, t.GetYear());
}

TEST(Date, Create_WhenDayIs0) {
  Date t;

  ASSERT_THROW(t.Set(0, 5, 2016), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, Create_WhenDayIs32) {
  Date t;

  ASSERT_THROW(t.Set(32, 5, 2016), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, Create_WhenMonthIs0) {
  Date t;

  ASSERT_THROW(t.Set(1, 0, 2016), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, Create_WhenMonthIs13) {
  Date t;

  ASSERT_THROW(t.Set(1, 13, 2016), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, Create_WhenYearIs1969) {
  Date t;

  ASSERT_THROW(t.Set(1, 13, 1969), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, Set_CorrectDate) {
  Date t;

  t.Set(8, 5, 2016);

  ASSERT_EQ(8, t.GetDay());
  ASSERT_EQ(5, t.GetMonth());
  ASSERT_EQ(2016, t.GetYear());
}

TEST(Date, Set_WhenDayIs0) {
  Date t;

  ASSERT_THROW(t.Set(0, 5, 2016), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, Set_WhenDayIs32) {
  Date t;

  ASSERT_THROW(t.Set(32, 5, 2016), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, Set_WhenMonthIs0) {
  Date t;

  ASSERT_THROW(t.Set(1, 0, 2016), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, Set_WhenMonthIs13) {
  Date t;

  ASSERT_THROW(t.Set(1, 13, 2016), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, Set_WhenYearIs1969) {
  Date t;

  ASSERT_THROW(t.Set(1, 13, 1969), ::type::exception::detail::WrongDateValueException);
}

TEST(Date, GetYesterdayDate_MiddleOfTheMonth) {
  Date t1 = Date::Create(22, 1, 2016);

  auto yesterday = t1.GetYesterdayDate();

  EXPECT_EQ(Date::Create(21, 1, 2016), yesterday);
}

TEST(Date, GetYesterdayDate_FirstDayOfMonth) {
  Date t1 = Date::Create(1, 4, 2016);

  auto yesterday = t1.GetYesterdayDate();

  EXPECT_EQ(Date::Create(31, 3, 2016), yesterday);
}

TEST(Date, GetYesterdayDate_FirstDayOfMonth_March_LeapYear) {
  Date t1 = Date::Create(1, 3, 2000);

  auto yesterday = t1.GetYesterdayDate();

  EXPECT_EQ(Date::Create(29, 2, 2000), yesterday);
}

TEST(Date, GetYesterdayDate_FirstDayOfMonth_March_NonLeapYear) {
  Date t1 = Date::Create(1, 3, 2100);

  auto yesterday = t1.GetYesterdayDate();

  EXPECT_EQ(Date::Create(28, 2, 2100), yesterday);
}

TEST(Date, GetYesterdayDate_FirstDayOfJanuary) {
  Date t1 = Date::Create(1, 1, 2016);

  auto yesterday = t1.GetYesterdayDate();

  EXPECT_EQ(Date::Create(31, 12, 2015), yesterday);
}

TEST(Date, GetTomorrowDate_MiddleOfTheMonth) {
  Date t1 = Date::Create(23, 1, 2016);

  auto tomorrow = t1.GetTomorrowDate();

  EXPECT_EQ(Date::Create(24, 1, 2016), tomorrow);
}

TEST(Date, GetTomorrowDate_EndOfTheMonth) {
  Date t1 = Date::Create(31, 1, 2016);

  auto tomorrow = t1.GetTomorrowDate();

  EXPECT_EQ(Date::Create(1, 2, 2016), tomorrow);
}

TEST(Date, GetTomorrowDate_FirstDayOfMonth_March_LeapYear) {
  Date t1 = Date::Create(29, 2, 2000);

  auto tomorrow = t1.GetTomorrowDate();

  EXPECT_EQ(Date::Create(1, 3, 2000), tomorrow);
}

TEST(Date, GetTomorrowDate_FirstDayOfMonth_March_NonLeapYear) {
  Date t1 = Date::Create(28, 2, 2100);

  auto tomorrow = t1.GetTomorrowDate();

  EXPECT_EQ(Date::Create(1, 3, 2100), tomorrow);
}

TEST(Date, GetTomorrowDate_LastDayOfTheYear) {
  Date t1 = Date::Create(31, 12, 2016);

  auto tomorrow = t1.GetTomorrowDate();

  EXPECT_EQ(Date::Create(1, 1, 2017), tomorrow);
}

TEST(Date, OperatorLess0) {
  Date t1 = Date::Create(1, 1, 2016);
  Date t2 = Date::Create(1, 1, 2016);

  EXPECT_FALSE(t1 < t2);
}

TEST(Date, OperatorLess1) {
  Date t1 = Date::Create(1, 1, 2016);
  Date t2 = Date::Create(2, 1, 2016);

  EXPECT_TRUE(t1 < t2);
}

TEST(Date, OperatorLess2) {
  Date t1 = Date::Create(1, 1, 2016);
  Date t2 = Date::Create(1, 2, 2016);

  EXPECT_TRUE(t1 < t2);
}

TEST(Date, OperatorLess3) {
  Date t1 = Date::Create(1, 1, 2016);
  Date t2 = Date::Create(1, 1, 2017);

  EXPECT_TRUE(t1 < t2);
}

TEST(Date, OperatorLess4) {
  Date t1 = Date::Create(3, 1, 2016);
  Date t2 = Date::Create(1, 2, 2016);

  EXPECT_TRUE(t1 < t2);
}

TEST(Date, OperatorLess5) {
  Date t1 = Date::Create(3, 7, 2015);
  Date t2 = Date::Create(1, 2, 2016);

  EXPECT_TRUE(t1 < t2);
}

TEST(Date, OperatorLess6) {
  Date t1 = Date::Create(2, 1, 2016);
  Date t2 = Date::Create(1, 1, 2016);

  EXPECT_FALSE(t1 < t2);
}

TEST(Date, OperatorLess7) {
  Date t1 = Date::Create(1, 2, 2016);
  Date t2 = Date::Create(1, 1, 2016);

  EXPECT_FALSE(t1 < t2);
}

TEST(Date, OperatorLess8) {
  Date t1 = Date::Create(1, 1, 2017);
  Date t2 = Date::Create(1, 1, 2016);

  EXPECT_FALSE(t1 < t2);
}
