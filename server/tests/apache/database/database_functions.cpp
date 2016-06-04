/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <gmock/gmock.h>

#include "src/apache/database/database_functions.h"

#include "tests/mock/database/sqlite_wrapper.h"
#include "tests/mock/database/general_database_functions.h"

using namespace testing;
using namespace std;

#define DB_STATEMENT_EXAMPLE_PTR_VALUE (reinterpret_cast<sqlite3_stmt*>(0x000002))

class apache_database_DatabaseFunctionsTest : public ::testing::Test {
 public:
  virtual ~apache_database_DatabaseFunctionsTest() = default;

  ::mock::database::SQLiteWrapperPtr sqlite_wrapper;
  ::mock::database::GeneralDatabaseFunctionsPtr general_database_functions;
  ::apache::database::DatabaseFunctionsPtr database_functions;
  ::type::Date example_date;

  void SetUp() {
    sqlite_wrapper = ::mock::database::SQLiteWrapper::Create();
    general_database_functions = ::mock::database::GeneralDatabaseFunctions::Create();
    database_functions = ::apache::database::DatabaseFunctions::Create(nullptr,
                                                                       sqlite_wrapper,
                                                                       general_database_functions);
    example_date = ::type::Date::Create(3, 1, 2016);
  }

  void TearDown() {
  }
};

TEST_F(apache_database_DatabaseFunctionsTest, MarkStatisticsAsCreatedFor) {
  EXPECT_CALL(*general_database_functions, AddAndGetDateId(example_date)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_wrapper, Exec(_, nullptr, nullptr));

  database_functions->MarkStatisticsAsCreatedFor(example_date);
}

TEST_F(apache_database_DatabaseFunctionsTest, AreStatisticsCreatedFor) {
  EXPECT_CALL(*general_database_functions, GetDateId(example_date)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_TRUE(database_functions->AreStatisticsCreatedFor(example_date));
}

TEST_F(apache_database_DatabaseFunctionsTest, AreStatisticsCreatedFor_WhenDateNotExists) {
  EXPECT_CALL(*general_database_functions, GetDateId(example_date)).WillOnce(Return(-1));

  EXPECT_FALSE(database_functions->AreStatisticsCreatedFor(example_date));
}

TEST_F(apache_database_DatabaseFunctionsTest, AreStatisticsCreatedFor_WhenRowNotExist) {
  EXPECT_CALL(*general_database_functions, GetDateId(example_date)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_FALSE(database_functions->AreStatisticsCreatedFor(example_date));
}

TEST_F(apache_database_DatabaseFunctionsTest, AreStatisticsCreatedFor_WhenStatisticsNotExists) {
  EXPECT_CALL(*general_database_functions, GetDateId(example_date)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(0));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_FALSE(database_functions->AreStatisticsCreatedFor(example_date));
}
