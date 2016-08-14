/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <gtest/gtest.h>

#include <slas/util/distance.h>

using namespace testing;
using namespace std;
using namespace util;

TEST(UtilTest, Distance_Seconds) {
  ::type::Time t1 = ::type::Time::Create(10, 1, 20);
  ::type::Time t2 = ::type::Time::Create(10, 1, 25);
  
  EXPECT_EQ(5, ::util::Distance(t2, t1));
}

TEST(UtilTest, Distance_Seconds_FirstT1) {
  ::type::Time t1 = ::type::Time::Create(10, 1, 20);
  ::type::Time t2 = ::type::Time::Create(10, 1, 25);
  
  EXPECT_EQ(5, ::util::Distance(t1, t2));
}

TEST(UtilTest, Distance_Minute) {
  ::type::Time t1 = ::type::Time::Create(10, 1, 20);
  ::type::Time t2 = ::type::Time::Create(10, 12, 20);
  
  EXPECT_EQ(660, ::util::Distance(t2, t1));
}

TEST(UtilTest, Distance_Minute_FirstT1) {
  ::type::Time t1 = ::type::Time::Create(10, 1, 20);
  ::type::Time t2 = ::type::Time::Create(10, 12, 20);
  
  EXPECT_EQ(660, ::util::Distance(t1, t2));
}

TEST(UtilTest, Distance_Minute_WithSeconds) {
  ::type::Time t1 = ::type::Time::Create(10, 1, 30);
  ::type::Time t2 = ::type::Time::Create(10, 2, 20);
  
  EXPECT_EQ(50, ::util::Distance(t2, t1));
}

TEST(UtilTest, Distance_Minute_WithSeconds_FirstT1) {
  ::type::Time t1 = ::type::Time::Create(10, 1, 30);
  ::type::Time t2 = ::type::Time::Create(10, 2, 20);
  
  EXPECT_EQ(50, ::util::Distance(t1, t2));
}

TEST(UtilTest, Distance_Hour) {
  ::type::Time t1 = ::type::Time::Create(10, 1, 20);
  ::type::Time t2 = ::type::Time::Create(12, 1, 20);
  
  EXPECT_EQ(7200, ::util::Distance(t2, t1));
}

TEST(UtilTest, Distance_Hour_FirstT1) {
  ::type::Time t1 = ::type::Time::Create(10, 1, 20);
  ::type::Time t2 = ::type::Time::Create(12, 1, 20);
  
  EXPECT_EQ(7200, ::util::Distance(t1, t2));
}

TEST(UtilTest, Distance_Hour_WithMinutes) {
  ::type::Time t1 = ::type::Time::Create(10, 2, 20);
  ::type::Time t2 = ::type::Time::Create(12, 1, 20);
  
  EXPECT_EQ(7140, ::util::Distance(t2, t1));
}

TEST(UtilTest, Distance_Hour_WithMinutes_FirstT1) {
  ::type::Time t1 = ::type::Time::Create(10, 2, 20);
  ::type::Time t2 = ::type::Time::Create(12, 1, 20);
  
  EXPECT_EQ(7140, ::util::Distance(t1, t2));
}

TEST(UtilTest, Distance_Hour_WithMinutesAndSeconds) {
  ::type::Time t1 = ::type::Time::Create(10, 2, 40);
  ::type::Time t2 = ::type::Time::Create(12, 1, 20);
  
  EXPECT_EQ(7120, ::util::Distance(t2, t1));
}

TEST(UtilTest, Distance_Hour_WithMinutesAndSeconds_FirstT1) {
  ::type::Time t1 = ::type::Time::Create(10, 2, 40);
  ::type::Time t2 = ::type::Time::Create(12, 1, 20);
  
  EXPECT_EQ(7120, ::util::Distance(t1, t2));
}
