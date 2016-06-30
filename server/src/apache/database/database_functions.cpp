/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "database_functions.h"

#include <algorithm>
#include <cmath>
#include <string>
#include <utility>
#include <boost/log/trivial.hpp>

#include "src/database/exception/detail/item_not_found_exception.h"
#include "src/database/exception/detail/cant_execute_sql_statement_exception.h"
#include "src/database/sqlite_wrapper.h"

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

  sqlite_wrapper_->Exec("create table if not exists APACHE_LOGS_TABLE("
                        "  ID integer primary key, "
                        "  AGENT_NAME text,"
                        "  VIRTUALHOST text, "
                        "  CLIENT_IP text, "
                        "  UTC_HOUR integer, "
                        "  UTC_MINUTE integer, "
                        "  UTC_SECOND integer, "
                        "  UTC_DAY integer, "
                        "  UTC_MONTH integer, "
                        "  UTC_YEAR integer, "
                        "  REQUEST text, "
                        "  STATUS_CODE integer, "
                        "  BYTES integer, "
                        "  USER_AGENT text, "
                        "  USED_IN_STATISTICS integer default 0"
                        ");");

  sqlite_wrapper_->Exec("create table if not exists APACHE_LAST_RUN_TABLE ( "
                        "  ID integer primary key not null, "
                        "  DATE_ID integer not null, "
                        "  TIME_ID integer not null, "
                        "  foreign key(DATE_ID) references DATE_TABLE(ID), "
                        "  foreign key(TIME_ID) references TIME_TABLE(ID) "
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

  sqlite_wrapper_->Exec("create table if not exists APACHE_SESSION_TABLE ("
                        "  ID integer primary key, "
                        "  AGENT_NAME text,"
                        "  VIRTUALHOST text, "
                        "  CLIENT_IP text, "
                        "  UTC_HOUR integer, "
                        "  UTC_MINUTE integer, "
                        "  UTC_SECOND integer, "
                        "  UTC_DAY integer, "
                        "  UTC_MONTH integer, "
                        "  UTC_YEAR integer, "
                        "  SESSION_LENGTH integer, "
                        "  BANDWIDTH_USAGE integer, "
                        "  REQUESTS_COUNT integer, "
                        "  ERRORS_COUNT integer, "
                        "  ERROR_PERCENTAGE real, "
                        "  USER_AGENT text, "
                        "  CLASSIFICATION integer default 0 "
                        ");");

  sqlite_wrapper_->Exec("create table if not exists APACHE_ANOMALY_DETECTION_CONFIGURATION_TABLE ("
                        "  ID integer primary key, "
                        "  AGENT_NAME text not null, "
                        "  VIRTUALHOST_NAME text not null, "
                        "  BEGIN_DATE_ID integer not null, "
                        "  END_DATE_ID integer not null, "
                        "  foreign key(BEGIN_DATE_ID) references DATE_TABLE(ID), "
                        "  foreign key(BEGIN_DATE_ID) references DATE_TABLE(ID),"
                        "  unique (AGENT_NAME, VIRTUALHOST_NAME) "
                        ");");
}

void DatabaseFunctions::RemoveAnomalyDetectionConfiguration(const ::database::type::RowId &id) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::RemoveAnomalyDetectionConfiguration: Function call";

  sqlite_wrapper_->Exec("delete from APACHE_ANOMALY_DETECTION_CONFIGURATION_TABLE "
                        " where ID=" + to_string(id));
}

const ::apache::type::AnomalyDetectionConfiguration DatabaseFunctions::GetAnomalyDetectionConfigurations() {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetApacheAnomalyDetectionConfiguration: Function call";
  return db_->GetApacheAnomalyDetectionConfiguration();
}

void DatabaseFunctions::AddLogs(const ::type::ApacheLogs &log_entries) {
  BOOST_LOG_TRIVIAL(debug) << "apache::DatabaseFunctions::AddLogs: Function call";

  sqlite_wrapper_->Exec("begin transaction");

  try {
    for (const ::type::ApacheLogEntry &entry : log_entries) {
      const char *sql = "insert into APACHE_LOGS_TABLE(AGENT_NAME, VIRTUALHOST, CLIENT_IP, UTC_HOUR, UTC_MINUTE, UTC_SECOND, UTC_DAY, UTC_MONTH, UTC_YEAR, REQUEST, STATUS_CODE, BYTES, USER_AGENT) values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
      sqlite3_stmt *statement;
      sqlite_wrapper_->Prepare(sql, &statement);

      try {
        sqlite_wrapper_->BindText(statement, 1, entry.agent_name);
        sqlite_wrapper_->BindText(statement, 2, entry.virtualhost);
        sqlite_wrapper_->BindText(statement, 3, entry.client_ip);

        sqlite_wrapper_->BindInt(statement, 4, entry.time.GetTime().GetHour());
        sqlite_wrapper_->BindInt(statement, 5, entry.time.GetTime().GetMinute());
        sqlite_wrapper_->BindInt(statement, 6, entry.time.GetTime().GetSecond());
        sqlite_wrapper_->BindInt(statement, 7, entry.time.GetDate().GetDay());
        sqlite_wrapper_->BindInt(statement, 8, entry.time.GetDate().GetMonth());
        sqlite_wrapper_->BindInt(statement, 9, entry.time.GetDate().GetYear());

        sqlite_wrapper_->BindText(statement, 10, entry.request);
        sqlite_wrapper_->BindInt(statement, 11, entry.status_code);
        sqlite_wrapper_->BindInt(statement, 12, entry.bytes);
        sqlite_wrapper_->BindText(statement, 13, entry.user_agent);

        sqlite_wrapper_->Step(statement);
      }
      catch (exception::DatabaseException &ex) {
        sqlite_wrapper_->Finalize(statement);
        throw;
      }

      sqlite_wrapper_->Finalize(statement);
    }

    sqlite_wrapper_->Exec("end transaction");
  }
  catch (exception::DatabaseException &ex) {
    sqlite_wrapper_->Exec("rollback");
    throw;
  }
}

::database::type::RowsCount DatabaseFunctions::GetUnusedLogsCount(std::string agent_name, std::string virtualhost_name) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetLogsCount: Function call";

  string sql =
      "select count(*) from APACHE_LOGS_TABLE "
      "  where "
      "    ( "
      "      AGENT_NAME=? "
      "      and "
      "      VIRTUALHOST=? "
      "    )"
      "  and "
      "  USED_IN_STATISTICS=0 "
      ";";

  sqlite3_stmt *statement;
  sqlite_wrapper_->Prepare(sql, &statement);

  sqlite_wrapper_->BindText(statement, 1, agent_name);

  sqlite_wrapper_->BindText(statement, 2, virtualhost_name);

  sqlite_wrapper_->Step(statement);

  auto count = sqlite_wrapper_->ColumnInt64(statement, 0);

  sqlite_wrapper_->Finalize(statement);

  return count;
}

::type::ApacheLogs DatabaseFunctions::GetUnusedLogs(std::string agent_name, std::string virtualhost_name,
                                                    unsigned limit, ::database::type::RowsCount offset) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetUnusedLogs: Function call";

  string sql =
      "select ID, AGENT_NAME, VIRTUALHOST, CLIENT_IP, UTC_HOUR, UTC_MINUTE, UTC_SECOND, UTC_DAY, UTC_MONTH, UTC_YEAR, REQUEST, STATUS_CODE, BYTES, USER_AGENT from APACHE_LOGS_TABLE"
      "  where"
      "    ("
      "      AGENT_NAME=?"
      "      and"
      "      VIRTUALHOST=?"
      "    )"
      "  and "
      "  USED_IN_STATISTICS=0 "
      "   limit " + to_string(limit) + " offset " + to_string(offset) +
      ";";

  sqlite3_stmt *statement;
  sqlite_wrapper_->Prepare(sql, &statement);

  sqlite_wrapper_->BindText(statement, 1, agent_name);
  sqlite_wrapper_->BindText(statement, 2, virtualhost_name);

  ::type::ApacheLogs logs;
  int ret;
  do {
    ret = sqlite_wrapper_->Step(statement);

    if (ret == SQLITE_ROW) {
      BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetUnusedLogs: Found new log entry in database";
      ::type::ApacheLogEntry log_entry;

      log_entry.id = sqlite_wrapper_->ColumnInt64(statement, 0);
      log_entry.agent_name = sqlite_wrapper_->ColumnText(statement, 1);
      log_entry.virtualhost = sqlite_wrapper_->ColumnText(statement, 2);
      log_entry.client_ip = sqlite_wrapper_->ColumnText(statement, 3);

      log_entry.time.Set(sqlite_wrapper_->ColumnInt(statement, 4),
                         sqlite_wrapper_->ColumnInt(statement, 5),
                         sqlite_wrapper_->ColumnInt(statement, 6),
                         sqlite_wrapper_->ColumnInt(statement, 7),
                         sqlite_wrapper_->ColumnInt(statement, 8),
                         sqlite_wrapper_->ColumnInt(statement, 9));

      log_entry.request = sqlite_wrapper_->ColumnText(statement, 10);
      log_entry.status_code = sqlite_wrapper_->ColumnInt(statement, 11);
      log_entry.bytes = sqlite_wrapper_->ColumnInt(statement, 12);
      log_entry.user_agent = sqlite_wrapper_->ColumnText(statement, 13);

      logs.push_back(log_entry);
    }
  }
  while (ret != SQLITE_DONE);

  sqlite_wrapper_->Finalize(statement);

  return logs;
}

void DatabaseFunctions::MarkLogsAsUsed(const ::type::ApacheLogs &logs) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::MarkLogsAsUsed: Function call";

  string sql = "update APACHE_LOGS_TABLE set USED_IN_STATISTICS=1 where ID in ( ";

  for (auto log : logs) {
    sql += to_string(log.id) + ", ";
  }

  sql += "-1 );";

  sqlite_wrapper_->Exec(sql);
}

bool DatabaseFunctions::AddSessionStatistics(const ::apache::type::ApacheSessions &sessions) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::AddSessionStatistics: Function call";

  return db_->AddApacheSessionStatistics(sessions);
}

::database::type::RowsCount DatabaseFunctions::GetSessionStatisticsCount(const std::string &agent_name, const std::string &virtualhost_name,
                                                                         const ::type::Timestamp &from, const ::type::Timestamp &to) {
  return db_->GetApacheSessionStatisticsCount(agent_name, virtualhost_name, from, to);
}

::apache::type::ApacheSessions DatabaseFunctions::GetSessionStatistics(const std::string &agent_name, const std::string &virtualhost_name,
                                                                       const ::type::Timestamp &from, const ::type::Timestamp &to,
                                                                       unsigned limit, ::database::type::RowsCount offset) {
  return db_->GetApacheSessionStatistics(agent_name, virtualhost_name, from, to, limit, offset);
}

bool DatabaseFunctions::IsSessionStatisticsWithoutLearningSetExists(const std::string &agent_name, const std::string &virtualhost_name) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::IsSessionStatisticsWithoutLearningSetExists: Function call";

  auto agent_id = general_database_functions_->GetAgentNameId(agent_name);
  auto virtualhost_id = GetVirtualhostNameId(virtualhost_name);

  string sql =
      "select count(1) "
      " from APACHE_SESSION_TABLE "
      "  where"
      "    ("
      "      AGENT_NAME=?"
      "      and"
      "      VIRTUALHOST=?"
      "    )"
      "  and "
      " ID not in ( select SESSION_ID from APACHE_LEARNING_SESSIONS where "
      "                 AGENT_NAME_ID=" + to_string(agent_id) + " and VIRTUALHOST_NAME_ID=" + to_string(virtualhost_id) +
      "            ) "
      ";";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  sqlite_wrapper_->BindText(statement, 1, agent_name);
  sqlite_wrapper_->BindText(statement, 2, virtualhost_name);

  int ret = sqlite_wrapper_->Step(statement);
  ::database::type::RowsCount count = 0;
  if (ret == SQLITE_ROW)
    count = sqlite_wrapper_->ColumnInt64(statement, 0);

  sqlite_wrapper_->Finalize(statement);

  return count;
}

::database::type::RowsCount DatabaseFunctions::GetSessionStatisticsWithoutLearningSetCount(const std::string &agent_name, const std::string &virtualhost_name,
                                                                                           const ::type::Timestamp &from, const ::type::Timestamp &to) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetApacheSessionStatisticsWithoutLearningSetCount: Function call";

  auto agent_id = general_database_functions_->GetAgentNameId(agent_name);
  auto virtualhost_id = GetVirtualhostNameId(virtualhost_name);

  string sql =
      "select count(*) "
      " from APACHE_SESSION_TABLE "
      "  where"
      "    ("
      "      AGENT_NAME=?"
      "      and"
      "      VIRTUALHOST=?"
      "    )"
      "  and "
      " ID not in ( select SESSION_ID from APACHE_LEARNING_SESSIONS where "
      "                 AGENT_NAME_ID=" + to_string(agent_id) + " and VIRTUALHOST_NAME_ID=" + to_string(virtualhost_id) +
      "            ) "
      "  and " +
      GetTimeRule(from, to) +
      ";";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  sqlite_wrapper_->BindText(statement, 1, agent_name);
  sqlite_wrapper_->BindText(statement, 2, virtualhost_name);

  int ret = sqlite_wrapper_->Step(statement);
  ::database::type::RowsCount count = 0;
  if (ret == SQLITE_ROW)
    count = sqlite_wrapper_->ColumnInt64(statement, 0);

  sqlite_wrapper_->Finalize(statement);

  return count;
}

::apache::type::ApacheSessions DatabaseFunctions::GetSessionStatisticsWithoutLearningSet(const std::string &agent_name, const std::string &virtualhost_name,
                                                                                         const ::type::Timestamp &from, const ::type::Timestamp &to,
                                                                                         unsigned limit, long long offset) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetApacheSessionStatisticsWithoutLearningSet: Function call";
  ::apache::type::ApacheSessions sessions;

  auto agent_id = general_database_functions_->GetAgentNameId(agent_name);
  auto virtualhost_id = GetVirtualhostNameId(virtualhost_name);

  string sql =
      "select ID, AGENT_NAME, VIRTUALHOST, CLIENT_IP, UTC_HOUR, UTC_MINUTE, UTC_SECOND, UTC_DAY, UTC_MONTH, UTC_YEAR, SESSION_LENGTH, BANDWIDTH_USAGE, REQUESTS_COUNT, ERRORS_COUNT, ERROR_PERCENTAGE, USER_AGENT, CLASSIFICATION "
      " from APACHE_SESSION_TABLE "
      "  where"
      "    ("
      "      AGENT_NAME=?"
      "      and"
      "      VIRTUALHOST=?"
      "    )"
      "  and "
      " ID not in ( select SESSION_ID from APACHE_LEARNING_SESSIONS where "
      "                 AGENT_NAME_ID=" + to_string(agent_id) + " and VIRTUALHOST_NAME_ID=" + to_string(virtualhost_id) +
      "            ) "
      "  and " +
      GetTimeRule(from, to) +
      "   limit " + to_string(limit) + " offset " + to_string(offset) +
      ";";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  sqlite_wrapper_->BindText(statement, 1, agent_name);
  sqlite_wrapper_->BindText(statement, 2, virtualhost_name);

  int ret;
  do {
    ret = sqlite_wrapper_->Step(statement);

    if (ret == SQLITE_ROW) {
      BOOST_LOG_TRIVIAL(debug) << "database::Database::GetApacheLogs: Found new log entry in database";
      ::apache::type::ApacheSessionEntry entry;

      entry.id = sqlite_wrapper_->ColumnInt64(statement, 0);
      entry.agent_name = sqlite_wrapper_->ColumnText(statement, 1);
      entry.virtualhost = sqlite_wrapper_->ColumnText(statement, 2);
      entry.client_ip = sqlite_wrapper_->ColumnText(statement, 3);

      auto t = ::type::Timestamp::Create(sqlite_wrapper_->ColumnInt(statement, 4),
                                         sqlite_wrapper_->ColumnInt(statement, 5),
                                         sqlite_wrapper_->ColumnInt(statement, 6),
                                         sqlite_wrapper_->ColumnInt(statement, 7),
                                         sqlite_wrapper_->ColumnInt(statement, 8),
                                         sqlite_wrapper_->ColumnInt(statement, 9));

      entry.session_start = t;
      entry.session_length = sqlite_wrapper_->ColumnInt64(statement, 10);
      entry.bandwidth_usage = sqlite_wrapper_->ColumnInt64(statement, 11);
      entry.requests_count = sqlite_wrapper_->ColumnInt64(statement, 12);
      entry.errors_count = sqlite_wrapper_->ColumnInt64(statement, 13);
      entry.error_percentage = sqlite_wrapper_->ColumnDouble(statement, 14);
      entry.useragent = sqlite_wrapper_->ColumnText(statement, 15);
      entry.classification = static_cast< ::database::type::Classification> (sqlite_wrapper_->ColumnInt(statement, 16));

      sessions.push_back(entry);
    }
  }
  while (ret != SQLITE_DONE);

  sqlite_wrapper_->Finalize(statement);

  return sessions;
}

::apache::type::ApacheSessionEntry DatabaseFunctions::GetOneSessionStatistic(::database::type::RowId id) {
  return db_->GetApacheOneSessionStatistic(id);
}

void DatabaseFunctions::MarkSessionStatisticAsAnomaly(const ::database::type::RowId &id) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::MarkSessionStatisticAsAnomaly: Function call";

  auto sql = "update APACHE_SESSION_TABLE "
      "set CLASSIFICATION=" + std::to_string(static_cast<int> (::database::type::Classification::ANOMALY)) +
      " where ID=" + to_string(id) + ";";

  sqlite_wrapper_->Exec(sql);
}

void DatabaseFunctions::ClearAnomalyMarksInLearningSet(const ::database::type::RowId &agent_name_id,
                                                       const ::database::type::RowId &virtualhost_name_id) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::ClearAnomalyMarksInLearningSet: Function call";

  auto sql =
      "update APACHE_SESSION_TABLE set CLASSIFICATION=" + std::to_string(static_cast<int> (::database::type::Classification::ANOMALY)) +
      " where "
      "  ID in ( "
      "         select SESSION_ID from APACHE_LEARNING_SESSIONS where "
      "             AGENT_NAME_ID=" + to_string(agent_name_id) +
      "           and "
      "             VIRTUALHOST_NAME_ID=" + to_string(virtualhost_name_id) +
      "         );";

  sqlite_wrapper_->Exec(sql);
}

void DatabaseFunctions::MarkLearningSetWithIqrMethod(const ::database::type::RowId &agent_name_id,
                                                     const ::database::type::RowId &virtualhost_name_id) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::MarkLearningSetWithIqrMethod: Function call";

  typedef std::pair<::database::type::RowId, long long> IdQueriesPair;
  std::vector<IdQueriesPair> id_queries;

  auto count = GetLearningSessionsCount(agent_name_id, virtualhost_name_id);
  auto learning_set_ids = GetLearningSessionsIds(agent_name_id, virtualhost_name_id, count, 0);

  for (auto id : learning_set_ids) {
    auto session = GetOneSessionStatistic(id);
    id_queries.push_back(std::make_pair(session.id, session.requests_count));
  }

  sort(id_queries.begin(), id_queries.end(), [](const IdQueriesPair &a, const IdQueriesPair & b) {
    return a.second < b.second;
  });

  auto q1_element = ceil(id_queries.size() * 1. / 4.) - 1;
  auto q3_element = ceil(id_queries.size() * 3. / 4.) - 1;
  auto iqr = id_queries.at(q3_element).second - id_queries.at(q1_element).second;

  auto m = id_queries.at(q3_element).second + 1.5 * iqr;

  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::MarkLearningSetWithIqrMethod: Info: q1_element=" << q1_element << "; q3_element=" << q3_element << "; iqr=" << iqr << "; m=" << m;

  ClearAnomalyMarksInLearningSet(agent_name_id, virtualhost_name_id);
  for (auto element : id_queries) {
    BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::MarkLearningSetWithIqrMethod: Loop: Comparing m to element.second=" << element.second;

    if (element.second > m)
      MarkSessionStatisticAsAnomaly(element.first);
  }
}

::database::type::AgentNames DatabaseFunctions::GetAgentNames() {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetAgentNames: Function call";

  return db_->GetApacheAgentNames();
}

::database::type::VirtualhostNames DatabaseFunctions::GetVirtualhostNames(::database::type::AgentName agent_name) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetVirtualhostNames: Function call";

  return db_->GetApacheVirtualhostNames(agent_name);
}

bool DatabaseFunctions::IsLastRunSet() {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::IsLastRunSet: Function call";

  const string sql =
      "select ID from APACHE_LAST_RUN_TABLE "
      "  where"
      "    ID=0;";

  auto id = sqlite_wrapper_->GetFirstInt64Column(sql, -1);

  return id != -1;
}

void DatabaseFunctions::SetLastRun(const ::type::Timestamp &timestamp) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::SetLastRun: Function call";

  auto date_id = general_database_functions_->AddAndGetDateId(timestamp.GetDate());
  auto time_id = general_database_functions_->AddAndGetTimeId(timestamp.GetTime());

  sqlite_wrapper_->Exec("insert or replace into APACHE_LAST_RUN_TABLE (ID, DATE_ID, TIME_ID) "
                        "values ("
                        " 0, "
                        + to_string(date_id) + ", "
                        + to_string(time_id)
                        + ");");
}

::type::Timestamp DatabaseFunctions::GetLastRun() {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetLastRun: Function call";

  const string date_sql =
      "select DATE_ID from APACHE_LAST_RUN_TABLE "
      "  where "
      "    ID=0 "
      ";";

  const string time_sql =
      "select TIME_ID from APACHE_LAST_RUN_TABLE "
      "  where "
      "    ID=0 "
      ";";

  auto date_id = sqlite_wrapper_->GetFirstInt64Column(date_sql);
  auto time_id = sqlite_wrapper_->GetFirstInt64Column(time_sql);

  return ::type::Timestamp::Create(general_database_functions_->GetTimeById(time_id),
                                   general_database_functions_->GetDateById(date_id));
}

void DatabaseFunctions::AddVirtualhostName(const std::string &name) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::AddVirtualhostName: Function call";

  string sql = "insert or ignore into APACHE_VIRTUALHOSTS_NAMES ( VIRTUALHOST_NAME ) values (?);";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    sqlite_wrapper_->BindText(statement, 1, name);
    sqlite_wrapper_->Step(statement);
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
    sqlite_wrapper_->BindText(statement, 1, name);

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

::apache::type::ApacheSessions DatabaseFunctions::GetLearningSessions(const ::database::type::RowId &agent, const ::database::type::RowId &virtualhost,
                                                                      unsigned limit, ::database::type::RowsCount offset) {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::GetApacheSessionStatistics: Function call";
  ::apache::type::ApacheSessions sessions;
  int ret;

  string sql =
      "select APACHE_SESSION_TABLE.ID, AGENT_NAME, VIRTUALHOST, CLIENT_IP, UTC_HOUR, UTC_MINUTE, UTC_SECOND, UTC_DAY, UTC_MONTH, UTC_YEAR, SESSION_LENGTH, BANDWIDTH_USAGE, REQUESTS_COUNT, ERRORS_COUNT, ERROR_PERCENTAGE, USER_AGENT, CLASSIFICATION "
      " from APACHE_SESSION_TABLE "
      " join APACHE_LEARNING_SESSIONS on APACHE_SESSION_TABLE.ID=APACHE_LEARNING_SESSIONS.SESSION_ID "
      "  where"
      "    ("
      "      APACHE_LEARNING_SESSIONS.AGENT_NAME_ID=" + to_string(agent) +
      "      and"
      "      APACHE_LEARNING_SESSIONS.VIRTUALHOST_NAME_ID=" + to_string(virtualhost) +
      "    )"
      "   limit " + to_string(limit) + " offset " + to_string(offset) +
      ";";

  sqlite3_stmt *statement;
  sqlite_wrapper_->Prepare(sql, &statement);

  do {
    ret = sqlite_wrapper_->Step(statement);

    if (ret == SQLITE_ROW) {
      BOOST_LOG_TRIVIAL(debug) << "database::Database::GetApacheLogs: Found new log entry in database";
      ::apache::type::ApacheSessionEntry entry;

      entry.id = sqlite_wrapper_->ColumnInt64(statement, 0);
      entry.agent_name = sqlite_wrapper_->ColumnText(statement, 1);
      entry.virtualhost = sqlite_wrapper_->ColumnText(statement, 2);
      entry.client_ip = sqlite_wrapper_->ColumnText(statement, 3);

      entry.session_start.Set(sqlite_wrapper_->ColumnInt(statement, 4),
                              sqlite_wrapper_->ColumnInt(statement, 5),
                              sqlite_wrapper_->ColumnInt(statement, 6),
                              sqlite_wrapper_->ColumnInt(statement, 7),
                              sqlite_wrapper_->ColumnInt(statement, 8),
                              sqlite_wrapper_->ColumnInt(statement, 9));

      entry.session_length = sqlite_wrapper_->ColumnInt64(statement, 10);
      entry.bandwidth_usage = sqlite_wrapper_->ColumnInt64(statement, 11);
      entry.requests_count = sqlite_wrapper_->ColumnInt64(statement, 12);
      entry.errors_count = sqlite_wrapper_->ColumnInt64(statement, 13);
      entry.error_percentage = sqlite_wrapper_->ColumnDouble(statement, 14);
      entry.useragent = sqlite_wrapper_->ColumnText(statement, 15);
      entry.classification = static_cast< ::database::type::Classification> (sqlite_wrapper_->ColumnInt(statement, 16));

      sessions.push_back(entry);
    }
  }
  while (ret != SQLITE_DONE);

  sqlite_wrapper_->Finalize(statement);

  return sessions;
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

void DatabaseFunctions::SetLearningSessions(const RowId &agent_id,
                                            const RowId &virtualhost_id,
                                            const RowIds &sessions_ids) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::SetLearningSessions: Function call";

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

void DatabaseFunctions::RemoveAllLearningSessions(const RowId &agent_id,
                                                  const RowId &virtualhost_id) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::RemoveAllLearningSessions: Function call";

  string sql =
      "delete from APACHE_LEARNING_SESSIONS where "
      "    AGENT_NAME_ID=" + to_string(agent_id) +
      "  and " +
      "    VIRTUALHOST_NAME_ID=" + to_string(virtualhost_id) +
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

string DatabaseFunctions::GetTimeRule(const ::type::Timestamp &from, const ::type::Timestamp &to) const {
  string from_day = to_string(from.GetDate().GetDay()),
      from_month = to_string(from.GetDate().GetMonth()),
      from_year = to_string(from.GetDate().GetYear()),
      from_hour = to_string(from.GetTime().GetHour()),
      from_minute = to_string(from.GetTime().GetMinute()),
      from_second = to_string(from.GetTime().GetSecond()),
      to_day = to_string(to.GetDate().GetDay()),
      to_month = to_string(to.GetDate().GetMonth()),
      to_year = to_string(to.GetDate().GetYear()),
      to_hour = to_string(to.GetTime().GetHour()),
      to_minute = to_string(to.GetTime().GetMinute()),
      to_second = to_string(to.GetTime().GetSecond());

  string rule =
      "  ("
      "    ("
      "      (UTC_YEAR > " + from_year + ")"
      "      or (UTC_YEAR = " + from_year + " and UTC_MONTH > " + from_month + ")"
      "      or (UTC_YEAR = " + from_year + " and UTC_MONTH = " + from_month + " and UTC_DAY > " + from_day + ")"
      "      or (UTC_YEAR = " + from_year + " and UTC_MONTH = " + from_month + " and UTC_DAY = " + from_day + " and UTC_HOUR > " + from_hour + ")"
      "      or (UTC_YEAR = " + from_year + " and UTC_MONTH = " + from_month + " and UTC_DAY = " + from_day + " and UTC_HOUR = " + from_hour + " and UTC_MINUTE > " + from_minute + ")"
      "      or (UTC_YEAR = " + from_year + " and UTC_MONTH = " + from_month + " and UTC_DAY = " + from_day + " and UTC_HOUR = " + from_hour + " and UTC_MINUTE = " + from_minute + " and UTC_SECOND >= " + from_second + ")"
      "    )"
      "  and"
      "    ("
      "      (UTC_YEAR < " + to_year + ")"
      "      or (UTC_YEAR = " + to_year + " and UTC_MONTH < " + to_month + ")"
      "      or (UTC_YEAR = " + to_year + " and UTC_MONTH = " + to_month + " and UTC_DAY < " + to_day + ")"
      "      or (UTC_YEAR = " + to_year + " and UTC_MONTH = " + to_month + " and UTC_DAY = " + to_day + " and UTC_HOUR < " + to_hour + ")"
      "      or (UTC_YEAR = " + to_year + " and UTC_MONTH = " + to_month + " and UTC_DAY = " + to_day + " and UTC_HOUR = " + to_hour + " and UTC_MINUTE < " + to_minute + ")"
      "      or (UTC_YEAR = " + to_year + " and UTC_MONTH = " + to_month + " and UTC_DAY = " + to_day + " and UTC_HOUR = " + to_hour + " and UTC_MINUTE = " + to_minute + " and UTC_SECOND <= " + to_second + ")"
      "    )"
      "  )"
      ;

  return rule;
}

}

}
