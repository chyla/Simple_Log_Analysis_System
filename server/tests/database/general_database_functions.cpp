/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <gmock/gmock.h>

#include "src/database/general_database_functions.h"
#include "src/database/exception/detail/cant_execute_sql_statement_exception.h"
#include "src/database/exception/detail/item_not_found_exception.h"

#include "tests/mock/database/sqlite_wrapper.h"

using namespace testing;
using namespace database;
using namespace std;

#define DB_STATEMENT_EXAMPLE_PTR_VALUE (reinterpret_cast<sqlite3_stmt*>(0x000002))

class GeneralDatabaseFunctionsTest : public ::testing::Test {
 public:
  ::mock::database::SQLiteWrapperPtr sqlite_wrapper;
  ::database::GeneralDatabaseFunctionsPtr general_database_functions;
  const string example_agent_name;

  GeneralDatabaseFunctionsTest() :
  example_agent_name("my_agent_name") {
  }

  virtual ~GeneralDatabaseFunctionsTest() = default;

  void SetUp() {
    sqlite_wrapper = ::mock::database::SQLiteWrapper::Create();
    general_database_functions = ::database::GeneralDatabaseFunctions::Create(sqlite_wrapper);
  }

  void TearDown() {
  }
};

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

TEST_F(GeneralDatabaseFunctionsTest, AddAndGetTimeId) {
  EXPECT_CALL(*sqlite_wrapper, GetFirstInt64Column(_, -1)).WillOnce(Return(11));

  auto id = general_database_functions->AddAndGetTimeId(::type::Time::Create(10, 12, 6));

  EXPECT_EQ(11, id);
}

TEST_F(GeneralDatabaseFunctionsTest, AddAndGetTimeId_WhenTimeNotFound) {
  EXPECT_CALL(*sqlite_wrapper, GetFirstInt64Column(_, -1)).WillOnce(Return(-1)).WillOnce(Return(12));
  EXPECT_CALL(*sqlite_wrapper, Exec(_, nullptr, nullptr));

  auto id = general_database_functions->AddAndGetTimeId(::type::Time::Create(10, 12, 6));

  EXPECT_EQ(12, id);
}

TEST_F(GeneralDatabaseFunctionsTest, AddAndGetDateId) {
  EXPECT_CALL(*sqlite_wrapper, GetFirstInt64Column(_, -1)).WillOnce(Return(11));

  auto id = general_database_functions->AddAndGetDateId(::type::Date::Create(10, 12, 2016));

  EXPECT_EQ(11, id);
}

TEST_F(GeneralDatabaseFunctionsTest, AddAndGetDateId_WhenDateNotFound) {
  EXPECT_CALL(*sqlite_wrapper, GetFirstInt64Column(_, -1)).WillOnce(Return(-1)).WillOnce(Return(12));
  EXPECT_CALL(*sqlite_wrapper, Exec(_, nullptr, nullptr));

  auto id = general_database_functions->AddAndGetDateId(::type::Date::Create(10, 12, 2016));

  EXPECT_EQ(12, id);
}

TEST_F(GeneralDatabaseFunctionsTest, GetDateById) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(11));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(12));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2)).WillOnce(Return(2016));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  auto date = general_database_functions->GetDateById(11);

  EXPECT_EQ(::type::Date::Create(11, 12, 2016), date);
}

TEST_F(GeneralDatabaseFunctionsTest, GetDateById_WhenTimeNotFound) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_THROW(general_database_functions->GetDateById(11), database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(GeneralDatabaseFunctionsTest, AddAgentName) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0, StrEq(example_agent_name.c_str())));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  general_database_functions->AddAgentName(example_agent_name);
}

TEST_F(GeneralDatabaseFunctionsTest, AddAgentName_WhenBindTextThrowException) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0, StrEq(example_agent_name.c_str()))).WillOnce(Throw(::database::exception::detail::CantExecuteSqlStatementException()));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_THROW(general_database_functions->AddAgentName(example_agent_name), ::database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(GeneralDatabaseFunctionsTest, AddAndGetAgentNameId) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0, StrEq(example_agent_name.c_str())));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_EQ(2, general_database_functions->AddAndGetAgentNameId(example_agent_name));
}

TEST_F(GeneralDatabaseFunctionsTest, AddAndGetAgentNameId_WhenAgentNameNotExists) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).Times(3).WillRepeatedly(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0, StrEq(example_agent_name.c_str()))).Times(3);
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(3));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(3);

  EXPECT_EQ(3, general_database_functions->AddAndGetAgentNameId(example_agent_name));
}

TEST_F(GeneralDatabaseFunctionsTest, GetAgentNameId) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0, StrEq(example_agent_name.c_str())));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_EQ(2, general_database_functions->GetAgentNameId(example_agent_name));
}

TEST_F(GeneralDatabaseFunctionsTest, GetAgentNameId_WhenRowNotFound) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0, StrEq(example_agent_name.c_str())));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_EQ(-1, general_database_functions->GetAgentNameId(example_agent_name));
}

TEST_F(GeneralDatabaseFunctionsTest, GetAgentNameId_WhenBindTextThrowException) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0, StrEq(example_agent_name.c_str()))).WillOnce(Throw(::database::exception::detail::CantExecuteSqlStatementException()));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_THROW(general_database_functions->GetAgentNameId(example_agent_name), ::database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(GeneralDatabaseFunctionsTest, GetAgentNameId_WhenStepThrowException) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0, StrEq(example_agent_name.c_str())));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Throw(::database::exception::detail::CantExecuteSqlStatementException()));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_THROW(general_database_functions->GetAgentNameId(example_agent_name), ::database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(GeneralDatabaseFunctionsTest, GetAgentNameById) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_wrapper, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(example_agent_name));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_EQ(example_agent_name, general_database_functions->GetAgentNameById(0));
}

TEST_F(GeneralDatabaseFunctionsTest, GetAgentNameById_WhenIdNotFound) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_THROW(general_database_functions->GetAgentNameById(0), ::database::exception::detail::ItemNotFoundException);
}

TEST_F(GeneralDatabaseFunctionsTest, GetAgentNameById_WhenStepThrowException) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Throw(::database::exception::detail::CantExecuteSqlStatementException()));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_THROW(general_database_functions->GetAgentNameById(0), ::database::exception::detail::CantExecuteSqlStatementException);
}
