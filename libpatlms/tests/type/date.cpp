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
