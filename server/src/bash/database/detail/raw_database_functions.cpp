/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "raw_database_functions.h"

#include <boost/log/trivial.hpp>

#include "src/database/exception/database_exception.h"

using namespace std;

namespace bash
{

namespace database
{

namespace detail
{

RawDatabaseFunctionsPtr RawDatabaseFunctions::Create(::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper) {
  return RawDatabaseFunctionsPtr(new RawDatabaseFunctions(sqlite_wrapper));
}

void RawDatabaseFunctions::CreateTables() {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::CreateTables: Function call";

  sqlite_wrapper_->Exec("create table if not exists BASH_COMMAND_TABLE ( "
                        "  ID integer primary key, "
                        "  COMMAND text, "
                        "  unique (COMMAND) "
                        ");");

  sqlite_wrapper_->Exec("create table if not exists BASH_SYSTEM_USER_TABLE ( "
                        "  ID integer primary key, "
                        "  SYSTEM_UID integer, "
                        "  unique (SYSTEM_UID) "
                        ");");

  sqlite_wrapper_->Exec("create table if not exists BASH_LOGS_TABLE ("
                        "  ID integer primary key, "
                        "  AGENT_NAME_ID integer, "
                        "  TIME_ID integer, "
                        "  DATE_ID integer, "
                        "  USER_ID integer, "
                        "  COMMAND_ID text, "
                        "  foreign key(AGENT_NAME_ID) references AGENT_NAMES(ID), "
                        "  foreign key(TIME_ID) references TIME_TABLE(ID), "
                        "  foreign key(DATE_ID) references DATE_TABLE(ID), "
                        "  foreign key(USER_ID) references SYSTEM_USER_TABLE(ID) "
                        ");");
}

void RawDatabaseFunctions::AddSystemUser(const entity::SystemUser &system_user) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::AddSystemUser: Function call";

  sqlite_wrapper_->Exec("insert or ignore into BASH_SYSTEM_USER_TABLE ( SYSTEM_UID ) values ( " +
                        to_string(system_user.uid) +
                        ");");
}

::database::type::RowId RawDatabaseFunctions::GetSystemUserId(const entity::SystemUser &system_user) {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::GetDateId: Function call";

  const string sql =
      "select ID from BASH_SYSTEM_USER_TABLE "
      "  where"
      "    SYSTEM_UID=" + to_string(system_user.uid) +
      ";";

  return sqlite_wrapper_->GetFirstInt64Column(sql, -1);
}

void RawDatabaseFunctions::AddCommand(const ::bash::database::type::CommandName &command) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::AddCommand: Function call";

  string sql =
      "insert or ignore into BASH_COMMAND_TABLE ( "
      "  COMMAND "
      ") "
      "values ( ? );";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    sqlite_wrapper_->BindText(statement, 1, command);
    sqlite_wrapper_->Step(statement);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::AddCommand: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);
}

::database::type::RowId RawDatabaseFunctions::GetCommandId(const ::bash::database::type::CommandName &command) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetCommandId: Function call";

  const char *sql =
      "select ID from BASH_COMMAND_TABLE "
      "  where"
      "    COMMAND=?"
      ";";

  ::database::type::RowId id = -1;

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    sqlite_wrapper_->BindText(statement, 1, command);

    auto ret = sqlite_wrapper_->Step(statement);
    if (ret == SQLITE_ROW)
      id = sqlite_wrapper_->ColumnInt64(statement, 0);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetCommandId: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return id;
}

void RawDatabaseFunctions::AddLog(const entity::Log &log) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::AddLog: Function call";

  string sql =
      "insert into BASH_LOGS_TABLE ( "
      "  AGENT_NAME_ID, "
      "  TIME_ID, "
      "  DATE_ID, "
      "  USER_ID, "
      "  COMMAND_ID "
      ") "
      "values ( " +
      to_string(log.agent_name_id) + ", " +
      to_string(log.time_id) + ", " +
      to_string(log.date_id) + ", " +
      to_string(log.user_id) + ", " +
      to_string(log.command_id) +
      ");";

  sqlite_wrapper_->Exec(sql);
}

RawDatabaseFunctions::RawDatabaseFunctions(::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper) :
sqlite_wrapper_(sqlite_wrapper) {
}

}

}

}
