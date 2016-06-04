/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "general_database_functions.h"

#include <iostream>
#include <boost/log/trivial.hpp>

#include "exception/detail/cant_execute_sql_statement_exception.h"

using namespace std;

namespace database
{

GeneralDatabaseFunctionsPtr GeneralDatabaseFunctions::Create(detail::SQLiteWrapperInterfacePtr sqlite_wrapper) {
  return GeneralDatabaseFunctionsPtr(new GeneralDatabaseFunctions(sqlite_wrapper));
}

void GeneralDatabaseFunctions::CreateTables() {
  BOOST_LOG_TRIVIAL(debug) << "database::GeneralDatabaseFunctions::CreateTables: Function call";

  sqlite_wrapper_->Exec("create table if not exists TIME_TABLE ("
                        "  ID integer primary key, "
                        "  HOUR integer, "
                        "  MINUTE integer, "
                        "  SECOND integer, "
                        "  unique(HOUR, MINUTE, SECOND) "
                        ");");

  sqlite_wrapper_->Exec("create table if not exists DATE_TABLE ("
                        "  ID integer primary key, "
                        "  DAY integer, "
                        "  MONTH integer, "
                        "  YEAR integer, "
                        "  unique(DAY, MONTH, YEAR) "
                        ");");
}

void GeneralDatabaseFunctions::AddTime(const ::type::Time &t) {
  BOOST_LOG_TRIVIAL(debug) << "database::GeneralDatabaseFunctions::AddTime: Function call";

  sqlite_wrapper_->Exec("insert or replace into TIME_TABLE (ID, HOUR, MINUTE, SECOND) "
                        "values ( "
                        " (select ID from TIME_TABLE where "
                        "    HOUR=" + to_string(t.GetHour()) + " and "
                        "    MINUTE=" + to_string(t.GetMinute()) + " and "
                        "    SECOND=" + to_string(t.GetSecond()) + "),"
                        + to_string(t.GetHour()) + ", "
                        + to_string(t.GetMinute()) + ", "
                        + to_string(t.GetSecond())
                        + " );");
}

::database::type::RowId GeneralDatabaseFunctions::GetTimeId(const ::type::Time &t) {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::GetDateId: Function call";
  ::database::type::RowId id = -1;

  const string sql =
      "select id from TIME_TABLE "
      "  where"
      "    HOUR=" + to_string(t.GetHour()) + " and "
      "    MINUTE=" + to_string(t.GetMinute()) + " and "
      "    SECOND=" + to_string(t.GetSecond()) +
      ";";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  auto ret = sqlite_wrapper_->Step(statement);
  if (ret == SQLITE_ROW)
    id = sqlite_wrapper_->ColumnInt64(statement, 0);

  sqlite_wrapper_->Finalize(statement);

  return id;
}

const ::type::Time GeneralDatabaseFunctions::GetTimeById(::database::type::RowId id) {
  BOOST_LOG_TRIVIAL(debug) << "database::GeneralDatabaseFunctions::GetTimeById: Function call";
  ::type::Time t;
  bool found = false;

  string sql =
      "select HOUR, MINUTE, SECOND from TIME_TABLE "
      "  where"
      "    ID=" + to_string(id) + ";";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  auto ret = sqlite_wrapper_->Step(statement);
  if (ret == SQLITE_ROW) {
    t.Set(sqlite_wrapper_->ColumnInt(statement, 0),
          sqlite_wrapper_->ColumnInt(statement, 1),
          sqlite_wrapper_->ColumnInt(statement, 2));
    found = true;
  }

  sqlite_wrapper_->Finalize(statement);

  if (!found) {
    BOOST_LOG_TRIVIAL(error) << "database::GeneralDatabaseFunctions::GetTimeById: Time not found";
    throw exception::detail::CantExecuteSqlStatementException();
  }

  return t;
}

void GeneralDatabaseFunctions::AddDate(const ::type::Date &date) {
  BOOST_LOG_TRIVIAL(debug) << "database::GeneralDatabaseFunctions::AddDate: Function call";

  sqlite_wrapper_->Exec(" insert or ignore into DATE_TABLE (DAY, MONTH, YEAR) "
                        " values ( "
                        + to_string(date.GetDay()) + ", " + to_string(date.GetMonth()) + ", " + to_string(date.GetYear())
                        + " );");
}

::database::type::RowId GeneralDatabaseFunctions::GetDateId(const ::type::Date &date) {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::GetDateId: Function call";
  ::database::type::RowId id = -1;

  string sql =
      "select id from DATE_TABLE "
      "  where"
      "    DAY=" + to_string(date.GetDay()) + " and "
      "    MONTH=" + to_string(date.GetMonth()) + " and "
      "    YEAR=" + to_string(date.GetYear()) +
      ";";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  auto ret = sqlite_wrapper_->Step(statement);
  if (ret == SQLITE_ROW)
    id = sqlite_wrapper_->ColumnInt64(statement, 0);

  sqlite_wrapper_->Finalize(statement);

  return id;
}

::type::Date GeneralDatabaseFunctions::GetDateById(const ::database::type::RowId &id) {
  BOOST_LOG_TRIVIAL(debug) << "database::GeneralDatabaseFunctions::GetDateById: Function call";
  ::type::Date date;
  bool found = false;

  string sql =
      "select DAY, MONTH, YEAR from DATE_TABLE "
      "  where"
      "    ID=" + to_string(id) + ";";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  auto ret = sqlite_wrapper_->Step(statement);
  if (ret == SQLITE_ROW) {
    date.Set(sqlite_wrapper_->ColumnInt(statement, 0),
             sqlite_wrapper_->ColumnInt(statement, 1),
             sqlite_wrapper_->ColumnInt(statement, 2));
    found = true;
  }

  sqlite_wrapper_->Finalize(statement);

  if (!found) {
    BOOST_LOG_TRIVIAL(error) << "database::GeneralDatabaseFunctions::GetTimeById: Date with id=" << id << " not found";
    throw exception::detail::CantExecuteSqlStatementException();
  }

  return date;
}

GeneralDatabaseFunctions::GeneralDatabaseFunctions(detail::SQLiteWrapperInterfacePtr sqlite_wrapper) :
sqlite_wrapper_(sqlite_wrapper) {
}

}
