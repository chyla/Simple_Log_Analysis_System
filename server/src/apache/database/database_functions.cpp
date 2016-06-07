/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "database_functions.h"

#include <string>
#include <boost/log/trivial.hpp>

#include "src/database/exception/detail/item_not_found_exception.h"
#include "src/database/exception/detail/cant_execute_sql_statement_exception.h"

using ::type::Timestamp;
using ::type::Time;
using ::type::Date;

using namespace database;
using namespace database::type;
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
                        "  TIME_ID integer not null, "
                        "  foreign key(DATE_ID) references DATE_TABLE(ID), "
                        "  foreign key(TIME_ID) references TIME_TABLE(ID), "
                        "  unique (RUN_TYPE) "
                        ");");

  sqlite_wrapper_->Exec("create table if not exists APACHE_VIRTUALHOSTS_NAMES ( "
                        "  ID integer primary key not null, "
                        "  VIRTUALHOST_NAME text, "
                        "  unique (VIRTUALHOST_NAME) "
                        ");");

  sqlite_wrapper_->Exec("create table if not exists APACHE_LEARNING_SESSIONS ( "
                        "  ID integer primary key not null, "
                        "  AGENT_NAME_ID integer not null, "
                        "  VIRTUALHOST_NAME_ID integer not null, "
                        "  SESSION_ID integer not null, "
                        "  foreign key(AGENT_NAME_ID) references AGENT_NAMES(ID), "
                        "  foreign key(VIRTUALHOST_NAME_ID) references APACHE_VIRTUALHOSTS_NAMES(ID), "
                        "  foreign key(SESSION_ID) references APACHE_SESSION_TABLE(ID), "
                        "  unique(AGENT_NAME_ID, VIRTUALHOST_NAME_ID, SESSION_ID) "
                        ");");
}

::database::type::RowsCount DatabaseFunctions::GetLogsCount(std::string agent_name, std::string virtualhost_name,
                                                            ::type::Timestamp from, ::type::Timestamp to) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetLogsCount: Function call";

  return db_->GetApacheLogsCount(agent_name, virtualhost_name, from, to);
}

::type::ApacheLogs DatabaseFunctions::GetLogs(std::string agent_name, std::string virtualhost_name,
                                              ::type::Timestamp from, ::type::Timestamp to,
                                              unsigned limit, ::database::type::RowsCount offset) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetLogs: Function call";

  return db_->GetApacheLogs(agent_name, virtualhost_name,
                            from, to,
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

void DatabaseFunctions::SetLastRun(const ::apache::type::LastRunType &type, const ::type::Timestamp &timestamp) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::SetLastRun: Function call";

  auto date_id = general_database_functions_->AddAndGetDateId(timestamp.GetDate());
  auto time_id = general_database_functions_->AddAndGetTimeId(timestamp.GetTime());

  sqlite_wrapper_->Exec("insert or replace into APACHE_LAST_RUN_TABLE (ID, RUN_TYPE, DATE_ID, TIME_ID) "
                        "values ("
                        " ( select ID from APACHE_LAST_RUN_TABLE where RUN_TYPE=" + to_string(static_cast<int> (type)) + " ), "
                        + to_string(static_cast<int> (type)) + ", "
                        + to_string(date_id) + ", "
                        + to_string(time_id)
                        + ");");
}

::type::Timestamp DatabaseFunctions::GetLastRun(const ::apache::type::LastRunType &type) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetLastRun: Function call";

  const string date_sql =
      "select DATE_ID from APACHE_LAST_RUN_TABLE "
      "  where"
      "    RUN_TYPE=" + to_string(static_cast<int> (type)) +
      ";";

  const string time_sql =
      "select TIME_ID from APACHE_LAST_RUN_TABLE "
      "  where"
      "    RUN_TYPE=" + to_string(static_cast<int> (type)) +
      ";";

  ::database::type::RowId date_id = sqlite_wrapper_->GetFirstInt64Column(date_sql);
  ::database::type::RowId time_id = sqlite_wrapper_->GetFirstInt64Column(time_sql);

  return ::type::Timestamp::Create(general_database_functions_->GetTimeById(time_id),
                                   general_database_functions_->GetDateById(date_id));
}

void DatabaseFunctions::AddVirtualhostName(const std::string &name) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::AddVirtualhostName: Function call";

  string sql = "insert or ignore into APACHE_VIRTUALHOSTS_NAMES ( VIRTUALHOST_NAME ) values (?);";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    sqlite_wrapper_->BindText(statement, 0, name);
  }
  catch (exception::DatabaseException &ex) {
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);
}

::database::type::RowId DatabaseFunctions::AddAndGetVirtualhostNameId(const std::string &name) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::AddAndGetVirtualhostNameId: Function call";

  auto id = GetVirtualhostNameId(name);
  if (id == -1) {
    AddVirtualhostName(name);
    id = GetVirtualhostNameId(name);
  }

  return id;
}

::database::type::RowId DatabaseFunctions::GetVirtualhostNameId(const std::string &name) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::GetVirtualhostNameId: Function call";
  ::database::type::RowId id = -1;

  string sql = "select ID from APACHE_VIRTUALHOSTS_NAMES where VIRTUALHOST_NAME=?";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    sqlite_wrapper_->BindText(statement, 0, name);

    auto ret = sqlite_wrapper_->Step(statement);
    if (ret == SQLITE_ROW)
      id = sqlite_wrapper_->ColumnInt64(statement, 0);
  }
  catch (exception::DatabaseException &ex) {
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return id;
}

std::string DatabaseFunctions::GetVirtualhostNameById(const ::database::type::RowId &id) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::GetVirtualhostNameById: Function call";
  std::string name;
  bool found = false;

  string sql =
      "select VIRTUALHOST_NAME from APACHE_VIRTUALHOSTS_NAMES "
      "  where"
      "    ID=" + to_string(id) + ";";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    auto ret = sqlite_wrapper_->Step(statement);
    if (ret == SQLITE_ROW) {
      name = sqlite_wrapper_->ColumnText(statement, 0);
      found = true;
    }
  }
  catch (exception::DatabaseException &ex) {
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  if (!found) {
    BOOST_LOG_TRIVIAL(error) << "database::DatabaseFunctions::GetVirtualhostNameById: Item with id=" << id << " not found";
    throw exception::detail::ItemNotFoundException();
  }

  return name;
}

::database::type::RowIds DatabaseFunctions::GetLearningSessionsIds(const RowId &agent_id,
                                                                   const RowId &virtualhost_id,
                                                                   unsigned limit, RowId offset) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::GetLearningSessions: Function call";
  ::database::type::RowIds rows;

  string sql =
      "select SESSION_ID from APACHE_LEARNING_SESSIONS where "
      "    AGENT_NAME_ID=" + to_string(agent_id) +
      "  and " +
      "    VIRTUALHOST_NAME_ID=" + to_string(virtualhost_id) +
      "  limit " + to_string(limit) + " offset " + to_string(offset) +
      ";";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    int ret;
    ::database::type::RowId id;
    do {
      ret = sqlite_wrapper_->Step(statement);
      if (ret == SQLITE_ROW) {
        id = sqlite_wrapper_->ColumnInt64(statement, 0);
        rows.push_back(id);
      }
    }
    while (ret != SQLITE_DONE);
  }
  catch (exception::DatabaseException &ex) {
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return rows;
}

::database::type::RowsCount DatabaseFunctions::GetLearningSessionsCount(const RowId &agent_id,
                                                                        const RowId &virtualhost_id) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::GetLearningSessionsCount: Function call";

  string sql =
      "select count(*) from APACHE_LEARNING_SESSIONS where "
      "    AGENT_NAME_ID=" + to_string(agent_id) +
      "  and " +
      "    VIRTUALHOST_NAME_ID=" + to_string(virtualhost_id) +
      ";";

  return sqlite_wrapper_->GetFirstInt64Column(sql);
}

void DatabaseFunctions::SetLearningSessions(const std::string &agent_name,
                                            const std::string &virtualhost_name,
                                            const RowIds &sessions_ids) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::SetLearningSessions: Function call";

  auto agent_id = general_database_functions_->GetAgentNameId(agent_name);
  auto virtualhost_id = GetVirtualhostNameId(virtualhost_name);

  string sql;
  sql = "begin transaction; ";

  for (auto id : sessions_ids) {
    sql +=
        "insert or ignore into APACHE_LEARNING_SESSIONS ( AGENT_NAME_ID, VIRTUALHOST_NAME_ID, SESSION_ID ) "
        "values ( "
        + to_string(agent_id) + ", "
        + to_string(virtualhost_id) + ", "
        + to_string(id) +
        "); ";
  }

  sql += "end transaction; ";

  sqlite_wrapper_->Exec(sql);
}

void DatabaseFunctions::RemoveAllLearningSessions(const std::string &agent_name,
                                                  const std::string &virtualhost_name) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::RemoveAllLearningSessions: Function call";

  auto agent_id = general_database_functions_->GetAgentNameId(agent_name);
  auto virtualhost_id = GetVirtualhostNameId(virtualhost_name);

  string sql =
      "delete from APACHE_LEARNING_SESSIONS where "
      "  AGENT_NAME_ID=" + to_string(agent_id) +
      "and " +
      "  VIRTUALHOST_NAME_ID=" + to_string(virtualhost_id) +
      ";";

  sqlite_wrapper_->Exec(sql);
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
