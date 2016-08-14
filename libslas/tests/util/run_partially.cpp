/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <gtest/gtest.h>
#include <vector>

#include <slas/util/run_partially.h>

using namespace testing;
using namespace util;
using namespace std;

class RunPartiallyTest : public ::testing::Test {
 public:

  std::vector<std::pair<long long, long long>> parts;

  void SetUp() {
    parts.clear();
  }

  void TearDown() {
  }

  virtual ~RunPartiallyTest() {
  }
};

TEST_F(RunPartiallyTest, RunPartiallyTest_LessThenPartSize) {
  util::RunPartially(100, 23, [&](long long count, long long offset) {
    parts.push_back({count, offset});
  });

  EXPECT_EQ(1, parts.size());
  EXPECT_EQ(23, parts.at(0).first);
  EXPECT_EQ(0, parts.at(0).second);
}

TEST_F(RunPartiallyTest, RunPartiallyTest_OnePart) {
  util::RunPartially(100, 100, [&](long long count, long long offset) {
    parts.push_back({count, offset});
  });

  EXPECT_EQ(1, parts.size());
  EXPECT_EQ(100, parts.at(0).first);
  EXPECT_EQ(0, parts.at(0).second);
}

TEST_F(RunPartiallyTest, RunPartiallyTest_TwoParts) {
  util::RunPartially(100, 133, [&](long long count, long long offset) {
    parts.push_back({count, offset});
  });

  EXPECT_EQ(2, parts.size());
  EXPECT_EQ(100, parts.at(0).first);
  EXPECT_EQ(0, parts.at(0).second);
  EXPECT_EQ(33, parts.at(1).first);
  EXPECT_EQ(100, parts.at(1).second);
}

TEST_F(RunPartiallyTest, RunPartiallyTest_TwoFullParts) {
  util::RunPartially(100, 200, [&](long long count, long long offset) {
    parts.push_back({count, offset});
  });

  EXPECT_EQ(2, parts.size());
  EXPECT_EQ(100, parts.at(0).first);
  EXPECT_EQ(0, parts.at(0).second);
  EXPECT_EQ(100, parts.at(1).first);
  EXPECT_EQ(100, parts.at(1).second);
}

TEST_F(RunPartiallyTest, RunPartiallyTest_ThreeParts) {
  util::RunPartially(100, 212, [&](long long count, long long offset) {
    parts.push_back({count, offset});
  });

  EXPECT_EQ(3, parts.size());
  EXPECT_EQ(100, parts.at(0).first);
  EXPECT_EQ(0, parts.at(0).second);
  EXPECT_EQ(100, parts.at(1).first);
  EXPECT_EQ(100, parts.at(1).second);
  EXPECT_EQ(12, parts.at(2).first);
  EXPECT_EQ(200, parts.at(2).second);
}
