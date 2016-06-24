/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <gmock/gmock.h>

#include "src/apache/database/database_functions.h"
#include "src/database/exception/detail/item_not_found_exception.h"
#include "src/database/exception/detail/cant_execute_sql_statement_exception.h"

#include "tests/mock/database/sqlite_wrapper.h"
#include "tests/mock/database/general_database_functions.h"

using namespace testing;
using namespace std;

#define DB_STATEMENT_EXAMPLE_PTR_VALUE (reinterpret_cast<sqlite3_stmt*>(0x000002))

class apache_database_DatabaseFunctionsTest : public ::testing::Test {
 public:
  ::mock::database::SQLiteWrapperPtr sqlite_wrapper;
  ::mock::database::GeneralDatabaseFunctionsPtr general_database_functions;
  ::apache::database::DatabaseFunctionsPtr database_functions;
  ::type::Date example_date;
  const string example_agent_name;
  const string example_virtualhost_name;

  apache_database_DatabaseFunctionsTest() :
  example_agent_name("example_agent"),
  example_virtualhost_name("example_vh_name") {
  }
  virtual ~apache_database_DatabaseFunctionsTest() = default;

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

TEST_F(apache_database_DatabaseFunctionsTest, AddVirtualhostName) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq(example_virtualhost_name.c_str())));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  database_functions->AddVirtualhostName(example_virtualhost_name);
}

TEST_F(apache_database_DatabaseFunctionsTest, AddVirtualhostName_WhenBindTextThrowException) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq(example_virtualhost_name.c_str()))).WillOnce(Throw(::database::exception::detail::CantExecuteSqlStatementException()));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_THROW(database_functions->AddVirtualhostName(example_virtualhost_name), ::database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(apache_database_DatabaseFunctionsTest, AddAndGetVirtualhostNameId) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq(example_virtualhost_name.c_str())));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_EQ(2, database_functions->AddAndGetVirtualhostNameId(example_virtualhost_name));
}

TEST_F(apache_database_DatabaseFunctionsTest, AddAndGetVirtualhostNameId_WhenVirtualhostNameNotExists) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).Times(3).WillRepeatedly(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq(example_virtualhost_name.c_str()))).Times(3);
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE)).WillOnce(Return(SQLITE_DONE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(3));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(3);

  EXPECT_EQ(3, database_functions->AddAndGetVirtualhostNameId(example_virtualhost_name));
}

TEST_F(apache_database_DatabaseFunctionsTest, GetVirtualhostNameId) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq(example_virtualhost_name.c_str())));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_wrapper, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_EQ(2, database_functions->GetVirtualhostNameId(example_virtualhost_name));
}

TEST_F(apache_database_DatabaseFunctionsTest, GetVirtualhostNameId_WhenRowNotFound) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq(example_virtualhost_name.c_str())));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_EQ(-1, database_functions->GetVirtualhostNameId(example_virtualhost_name));
}

TEST_F(apache_database_DatabaseFunctionsTest, GetVirtualhostNameId_WhenBindTextThrowException) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq(example_virtualhost_name.c_str()))).WillOnce(Throw(::database::exception::detail::CantExecuteSqlStatementException()));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_THROW(database_functions->GetVirtualhostNameId(example_virtualhost_name), ::database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(apache_database_DatabaseFunctionsTest, GetVirtualhostNameId_WhenStepThrowException) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq(example_virtualhost_name.c_str())));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Throw(::database::exception::detail::CantExecuteSqlStatementException()));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_THROW(database_functions->GetVirtualhostNameId(example_virtualhost_name), ::database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(apache_database_DatabaseFunctionsTest, GetVirtualhostNameById) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_wrapper, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(example_virtualhost_name));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_EQ(example_virtualhost_name, database_functions->GetVirtualhostNameById(0));
}

TEST_F(apache_database_DatabaseFunctionsTest, GetVirtualhostNameById_WhenIdNotFound) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_THROW(database_functions->GetVirtualhostNameById(0), ::database::exception::detail::ItemNotFoundException);
}

TEST_F(apache_database_DatabaseFunctionsTest, GetVirtualhostNameById_WhenStepThrowException) {
  EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Throw(::database::exception::detail::CantExecuteSqlStatementException()));
  EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));

  EXPECT_THROW(database_functions->GetVirtualhostNameById(0), ::database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(apache_database_DatabaseFunctionsTest, GetLearningSessionsIds) {
  {
    InSequence s;

    EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
    EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
    EXPECT_CALL(*sqlite_wrapper, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(3));
    EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
    EXPECT_CALL(*sqlite_wrapper, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(8));
    EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
    EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  }

  auto ids = database_functions->GetLearningSessionsIds(1, 2, 10, 0);
  EXPECT_EQ(2, ids.size());
  EXPECT_EQ(3, ids.at(0));
  EXPECT_EQ(8, ids.at(1));
}

TEST_F(apache_database_DatabaseFunctionsTest, GetLearningSessionsIds_WhenNoItemsFound) {
  {
    InSequence s;

    EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
    EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
    EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  }

  auto ids = database_functions->GetLearningSessionsIds(1, 2, 10, 0);
  EXPECT_EQ(0, ids.size());
}

TEST_F(apache_database_DatabaseFunctionsTest, GetLearningSessionsIds_WhenStepThrowException) {
  {
    InSequence s;

    EXPECT_CALL(*sqlite_wrapper, Prepare(_, NotNull())).WillOnce(SetArgPointee<1>(DB_STATEMENT_EXAMPLE_PTR_VALUE));
    EXPECT_CALL(*sqlite_wrapper, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Throw(::database::exception::detail::CantExecuteSqlStatementException()));
    EXPECT_CALL(*sqlite_wrapper, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE));
  }

  EXPECT_THROW(database_functions->GetLearningSessionsIds(1, 2, 10, 0), ::database::exception::detail::CantExecuteSqlStatementException);
}
