/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <gmock/gmock.h>

#include "src/database/general_database_functions.h"
#include "src/database/exception/detail/cant_execute_sql_statement_exception.h"

#include "tests/mock/database/sqlite_wrapper.h"

using namespace testing;
using namespace database;
using namespace std;

#define DB_STATEMENT_EXAMPLE_PTR_VALUE (reinterpret_cast<sqlite3_stmt*>(0x000002))

class GeneralDatabaseFunctionsTest : public ::testing::Test {
 public:
  ::mock::database::SQLiteWrapperPtr sqlite_wrapper;
  ::database::GeneralDatabaseFunctionsPtr general_database_functions;

  virtual ~GeneralDatabaseFunctionsTest() = default;

  void SetUp() {
    sqlite_wrapper = ::mock::database::SQLiteWrapper::Create();
    general_database_functions = ::database::GeneralDatabaseFunctions::Create(sqlite_wrapper);
  }

  void TearDown() {
  }
};

TEST_F(GeneralDatabaseFunctionsTest, GetTimeId) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(11));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  auto id = general_database_functions->GetTimeId(::type::Time::Create(10, 12, 14));

  EXPECT_EQ(11, id);
}

TEST_F(GeneralDatabaseFunctionsTest, GetTimeId_WhenTimeNotFound) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  auto id = general_database_functions->GetTimeId(::type::Time::Create(10, 12, 14));

  EXPECT_EQ(-1, id);
}

TEST_F(GeneralDatabaseFunctionsTest, GetTimeById) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(11));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(12));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2)).WillOnce(Return(21));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  auto t = general_database_functions->GetTimeById(11);

  EXPECT_EQ(::type::Time::Create(11, 12, 21), t);
}

TEST_F(GeneralDatabaseFunctionsTest, GetTimeById_WhenTimeNotFound) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_THROW(general_database_functions->GetTimeById(11), database::exception::detail::CantExecuteSqlStatementException);
}
