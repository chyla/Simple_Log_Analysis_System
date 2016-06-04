/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "database_functions.h"

#include <string>
#include <boost/log/trivial.hpp>

#include "src/database/exception/detail/cant_execute_sql_statement_exception.h"

using ::type::Timestamp;
using ::type::Time;
using ::type::Date;

using namespace std;

namespace apache
{

namespace database
{

DatabaseFunctionsPtr DatabaseFunctions::Create(::database::DatabasePtr db,
                                               ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper,
                                               ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::Create: Function call";

  return DatabaseFunctionsPtr(new DatabaseFunctions(db, sqlite_wrapper, general_database_functions));
}

void DatabaseFunctions::CreateTables() {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::CreateTables: Function call";

  sqlite_wrapper_->Exec("create table if not exists APACHE_LAST_RUN_TABLE ( "
                        "  ID integer primary key not null, "
                        "  RUN_TYPE integer not null, "
                        "  DATE_ID integer not null, "
                        "  foreign key(DATE_ID) references DATE_TABLE(ID), "
                        "  unique (RUN_TYPE) "
                        ");");
}

::database::type::RowsCount DatabaseFunctions::GetLogsCount(std::string agent_name, std::string virtualhost_name,
                                                            ::type::Date from, ::type::Date to) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetLogsCount: Function call";

  return db_->GetApacheLogsCount(agent_name, virtualhost_name,
                                 Timestamp::Create(Time::Create(0, 0, 0), from),
                                 Timestamp::Create(Time::Create(23, 59, 59), to));
}

::type::ApacheLogs DatabaseFunctions::GetLogs(std::string agent_name, std::string virtualhost_name,
                                              ::type::Date from, ::type::Date to,
                                              unsigned limit, ::database::type::RowsCount offset) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetLogs: Function call";

  return db_->GetApacheLogs(agent_name, virtualhost_name,
                            Timestamp::Create(Time::Create(0, 0, 0), from),
                            Timestamp::Create(Time::Create(23, 59, 59), to),
                            limit, offset);
}

bool DatabaseFunctions::AddSessionStatistics(const ::apache::type::ApacheSessions &sessions) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::AddSessionStatistics: Function call";

  return db_->AddApacheSessionStatistics(sessions);
}

void DatabaseFunctions::MarkStatisticsAsCreatedFor(::type::Date date) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::MarkStatisticsAsCreatedFor: Function call";

  auto date_id = general_database_functions_->AddAndGetDateId(date);

  sqlite_wrapper_->Exec("insert or ignore into APACHE_SESSION_EXISTS_TABLE (DATE_ID, EXIST) "
                        "values ("
                        + to_string(date_id) + ", "
                        "  1 "
                        ");");
}

bool DatabaseFunctions::AreStatisticsCreatedFor(::type::Date date) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::IsStatisticsCreatedFor: Function call";
  bool created = false;

  auto date_id = general_database_functions_->GetDateId(date);
  if (date_id == -1)
    return false;

  string sql =
      "select EXIST from APACHE_SESSION_EXISTS_TABLE "
      "  where"
      "    DATE_ID=" + to_string(date_id) +
      ";";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  auto ret = sqlite_wrapper_->Step(statement);
  if (ret == SQLITE_ROW)
    created = static_cast<bool> (sqlite_wrapper_->ColumnInt(statement, 0));

  sqlite_wrapper_->Finalize(statement);

  return created;
}

::database::type::AgentNames DatabaseFunctions::GetAgentNames() {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetAgentNames: Function call";

  return db_->GetApacheAgentNames();
}

::database::type::VirtualhostNames DatabaseFunctions::GetVirtualhostNames(::database::type::AgentName agent_name) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetVirtualhostNames: Function call";

  return db_->GetApacheVirtualhostNames(agent_name);
}

bool DatabaseFunctions::IsLastRunSet(const ::apache::type::LastRunType &type) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::IsLastRunSet: Function call";

  const string sql =
      "select ID from APACHE_LAST_RUN_TABLE "
      "  where"
      "    RUN_TYPE=" + to_string(static_cast<int> (type)) +
      ";";

  auto id = sqlite_wrapper_->GetFirstInt64Column(sql, -1);

  return id != -1;
}

void DatabaseFunctions::SetLastRun(const ::apache::type::LastRunType &type, const ::type::Date &date) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::SetLastRun: Function call";

  auto date_id = general_database_functions_->AddAndGetDateId(date);

  sqlite_wrapper_->Exec("insert or replace into APACHE_LAST_RUN_TABLE (ID, RUN_TYPE, DATE_ID) "
                        "values ("
                        " ( select ID from APACHE_LAST_RUN_TABLE where RUN_TYPE=" + to_string(static_cast<int> (type)) + " ), "
                        + to_string(static_cast<int> (type)) + ", "
                        + to_string(date_id) + ", "
                        ");");
}

::type::Date DatabaseFunctions::GetLastRun(const ::apache::type::LastRunType &type) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetLastRun: Function call";

  const string sql =
      "select DATE_ID from APACHE_LAST_RUN_TABLE "
      "  where"
      "    RUN_TYPE=" + to_string(static_cast<int> (type)) +
      ";";

  ::database::type::RowId id = sqlite_wrapper_->GetFirstInt64Column(sql);

  return general_database_functions_->GetDateById(id);
}

DatabaseFunctions::DatabaseFunctions(::database::DatabasePtr db,
                                     ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper,
                                     ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) :
db_(db),
sqlite_wrapper_(sqlite_wrapper),
general_database_functions_(general_database_functions) {
}

}

}
