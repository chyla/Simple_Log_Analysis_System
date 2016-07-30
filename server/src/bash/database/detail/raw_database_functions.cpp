/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "raw_database_functions.h"

#include <boost/log/trivial.hpp>

#include "src/database/exception/database_exception.h"
#include "src/database/exception/detail/item_not_found_exception.h"
#include "src/database/type/classification.h"

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
                        "  COMMAND_ID integer, "
                        "  foreign key(AGENT_NAME_ID) references AGENT_NAMES(ID), "
                        "  foreign key(TIME_ID) references TIME_TABLE(ID), "
                        "  foreign key(DATE_ID) references DATE_TABLE(ID), "
                        "  foreign key(USER_ID) references SYSTEM_USER_TABLE(ID) "
                        ");");

  sqlite_wrapper_->Exec("create table if not exists BASH_DAILY_STATISTICS_TABLE ("
                        "  ID integer primary key, "
                        "  AGENT_NAME_ID integer, "
                        "  DATE_ID integer, "
                        "  COMMAND_ID integer, "
                        "  SUMMARY integer, "
                        "  foreign key(AGENT_NAME_ID) references AGENT_NAMES(ID), "
                        "  foreign key(DATE_ID) references DATE_TABLE(ID), "
                        "  foreign key(COMMAND_ID) references BASH_COMMAND_TABLE(ID) "
                        ");");

  sqlite_wrapper_->Exec("create table if not exists BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE ("
                        "  ID integer primary key, "
                        "  AGENT_NAME_ID integer, "
                        "  BEGIN_DATE_ID integer, "
                        "  END_DATE_ID integer, "
                        "  CHANGED integer, "
                        "  foreign key(AGENT_NAME_ID) references AGENT_NAMES(ID), "
                        "  foreign key(BEGIN_DATE_ID) references DATE_TABLE(ID), "
                        "  foreign key(END_DATE_ID) references DATE_TABLE(ID), "
                        "  unique (AGENT_NAME_ID) "
                        ");");

  sqlite_wrapper_->Exec("create table if not exists BASH_DATE_RANGE_COMMANDS_STATISTICS_TABLE ("
                        "  ID integer primary key, "
                        "  AGENT_NAME_ID integer, "
                        "  COMMAND_ID integer, "
                        "  BEGIN_DATE_ID integer, "
                        "  END_DATE_ID integer, "
                        "  SUMMARY integer, "
                        "  foreign key(AGENT_NAME_ID) references AGENT_NAMES(ID), "
                        "  foreign key(COMMAND_ID) references BASH_COMMAND_TABLE(ID), "
                        "  foreign key(BEGIN_DATE_ID) references DATE_TABLE(ID), "
                        "  foreign key(END_DATE_ID) references DATE_TABLE(ID), "
                        "  unique (AGENT_NAME_ID, COMMAND_ID, BEGIN_DATE_ID, END_DATE_ID) "
                        ");");

  sqlite_wrapper_->Exec("create table if not exists BASH_SELECTED_COMMANDS_TABLE ( "
                        "  ID integer primary key, "
                        "  CONFIGURATION_ID integer, "
                        "  COMMAND_ID integer, "
                        "  foreign key(CONFIGURATION_ID) references BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE(ID), "
                        "  foreign key(COMMAND_ID) references BASH_COMMAND_TABLE(ID), "
                        "  unique (CONFIGURATION_ID, COMMAND_ID) "
                        ");");

  sqlite_wrapper_->Exec("create table if not exists BASH_DAILY_USER_STATISTICS_TABLE ("
                        "  ID integer primary key, "
                        "  AGENT_NAME_ID integer, "
                        "  USER_ID integer, "
                        "  DATE_ID integer, "
                        "  CLASSIFICATION integer default 0, "
                        "  foreign key(AGENT_NAME_ID) references AGENT_NAMES(ID), "
                        "  foreign key(USER_ID) references BASH_SYSTEM_USER_TABLE(ID), "
                        "  foreign key(DATE_ID) references DATE_TABLE(ID) "
                        ");");

  sqlite_wrapper_->Exec("create table if not exists BASH_DAILY_USER_COMMAND_STATISTICS_TABLE ("
                        "  ID integer primary key, "
                        "  STATISTIC_ID integer, "
                        "  COMMAND_ID integer, "
                        "  SUMMARY integer, "
                        "  foreign key(STATISTIC_ID) references BASH_DAILY_USER_STATISTICS_TABLE(ID), "
                        "  foreign key(COMMAND_ID) references BASH_COMMAND_TABLE(ID) "
                        ");");

  sqlite_wrapper_->Exec("create table if not exists BASH_ANOMALY_DETECTION_CONFIGURATION_SELECTED_STATISTICS_TABLE ("
                        "  ID integer primary key, "
                        "  CONFIGURATION_ID integer, "
                        "  STATISTIC_ID integer, "
                        "  foreign key(CONFIGURATION_ID) references BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE(ID), "
                        "  foreign key(STATISTIC_ID) references BASH_DAILY_USER_STATISTICS_TABLE(ID), "
                        "  unique (CONFIGURATION_ID, STATISTIC_ID) "
                        ");");
}

void RawDatabaseFunctions::AddSystemUser(const entity::SystemUser &system_user) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::AddSystemUser: Function call";

  sqlite_wrapper_->Exec("insert or ignore into BASH_SYSTEM_USER_TABLE ( SYSTEM_UID ) values ( " +
                        to_string(system_user.uid) +
                        ");");
}

::database::type::RowId RawDatabaseFunctions::GetSystemUserId(const entity::SystemUser &system_user) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetSystemUserId: Function call";

  const string sql =
      "select ID from BASH_SYSTEM_USER_TABLE "
      "  where"
      "    SYSTEM_UID=" + to_string(system_user.uid) +
      ";";

  return sqlite_wrapper_->GetFirstInt64Column(sql, -1);
}

::bash::database::detail::entity::SystemUser RawDatabaseFunctions::GetSystemUserById(::database::type::RowId id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetSystemUserById: Function call";

  string sql = "select SYSTEM_UID from BASH_SYSTEM_USER_TABLE where ID=" + to_string(id) + ";";


  entity::SystemUser su{
    sqlite_wrapper_->GetFirstInt64Column(sql)
  };

  return su;
}

::database::type::RowIds RawDatabaseFunctions::GetSystemUsersIdsFromLogs(::database::type::RowId agent_name_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetSystemUsersIdsFromLogs: Function call";

  const string sql = "select distinct USER_ID from BASH_LOGS_TABLE "
      " where AGENT_NAME_ID= " + to_string(agent_name_id) +
      ";";

  ::database::type::RowIds ids;
  ::database::type::RowId id;

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        id = sqlite_wrapper_->ColumnInt64(statement, 0);
        ids.push_back(id);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetSystemUsersIdsFromLogs: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return ids;
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

::database::type::RowIds RawDatabaseFunctions::GetAllCommandsIds() {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetAllCommandsIds: Function call";

  ::database::type::RowIds commands_ids;
  ::database::type::RowId id;

  const char *sql = "select ID from BASH_COMMAND_TABLE;";


  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        id = sqlite_wrapper_->ColumnInt64(statement, 0);
        commands_ids.push_back(id);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetCommandId: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return commands_ids;
}

::bash::database::type::CommandName RawDatabaseFunctions::GetCommandNameById(::database::type::RowId id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetCommandNameById: Function call";

  string sql = "select COMMAND from BASH_COMMAND_TABLE where ID=" + to_string(id) + ";";
  ::bash::database::type::CommandName name;

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    auto ret = sqlite_wrapper_->Step(statement);

    if (ret == SQLITE_ROW) {
      name = sqlite_wrapper_->ColumnText(statement, 0);
    }
    else {
      BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetCommandNameById: Command name with id=" << id << " not found";
      throw ::database::exception::detail::ItemNotFoundException();
    }
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetCommandId: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return name;
}

void RawDatabaseFunctions::AddLog(const entity::Log & log) {
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

::database::type::RowsCount RawDatabaseFunctions::CountCommandsForDailySystemStatistic(::database::type::RowId agent_name_id,
                                                                                       ::database::type::RowId date_id,
                                                                                       ::database::type::RowId command_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::CountCommandsForDailySystemStatistic: Function call";

  string sql =
      "select count(*) from BASH_LOGS_TABLE "
      "  where "
      "    AGENT_NAME_ID=" + to_string(agent_name_id) +
      "  and "
      "    DATE_ID=" + to_string(date_id) +
      "  and "
      "    COMMAND_ID=" + to_string(command_id) +
      ";";

  return sqlite_wrapper_->GetFirstInt64Column(sql);
}

::database::type::RowsCount RawDatabaseFunctions::CountCommandsForUserDailyStatisticFromLogs(::database::type::RowId agent_name_id,
                                                                                             ::database::type::RowId date_id,
                                                                                             ::database::type::RowId user_id,
                                                                                             ::database::type::RowId command_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::CountCommandsForUserDailyStatisticFromLogs: Function call";

  string sql =
      "select count(*) from BASH_LOGS_TABLE "
      "  where "
      "    AGENT_NAME_ID=" + to_string(agent_name_id) +
      "  and "
      "    DATE_ID=" + to_string(date_id) +
      "  and "
      "    COMMAND_ID=" + to_string(command_id) +
      "  and "
      "    USER_ID=" + to_string(user_id) +
      ";";

  return sqlite_wrapper_->GetFirstInt64Column(sql);
}

void RawDatabaseFunctions::AddDailySystemStatistic(const entity::DailySystemStatistic & statistic) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::AddDailySystemStatistic: Function call";

  string sql =
      "insert into BASH_DAILY_STATISTICS_TABLE ( "
      "  AGENT_NAME_ID, "
      "  DATE_ID, "
      "  COMMAND_ID, "
      "  SUMMARY "
      ") "
      "values ( " +
      to_string(statistic.agent_name_id) + ", " +
      to_string(statistic.date_id) + ", " +
      to_string(statistic.command_id) + ", " +
      to_string(statistic.summary) +
      "); ";

  sqlite_wrapper_->Exec(sql);
}

::database::type::RowIds RawDatabaseFunctions::GetDateIdsWithoutCreatedDailySystemStatistic(::database::type::RowId agent_name_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetAllCommandsIds: Function call";

  ::database::type::RowIds date_ids;
  ::database::type::RowId id;

  string sql =
      "select DT.ID from DATE_TABLE as DT "
      "  left join BASH_DAILY_STATISTICS_TABLE as BDST "
      "  on DT.ID=BDST.DATE_ID and BDST.AGENT_NAME_ID=" + to_string(agent_name_id) +
      "  where BDST.DATE_ID is null;";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        id = sqlite_wrapper_->ColumnInt64(statement, 0);
        date_ids.push_back(id);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetCommandId: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return date_ids;
}

::database::type::RowIds RawDatabaseFunctions::GetAgentIdsWithoutConfiguration() {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetAgentIdsWithoutConfiguration: Function call";

  ::database::type::RowIds ids;
  ::database::type::RowId id;

  string sql =
      "select AN.ID from AGENT_NAMES as AN "
      " left join BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE as BADCT "
      " on AN.ID=BADCT.AGENT_NAME_ID where BADCT.ID is null;";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        id = sqlite_wrapper_->ColumnInt64(statement, 0);
        ids.push_back(id);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetAgentIdsWithoutConfiguration: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return ids;
}

::database::type::RowIds RawDatabaseFunctions::GetAgentsIdsWithConfiguration() {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetAgentsIdsWithConfiguration: Function call";

  ::database::type::RowIds ids;
  ::database::type::RowId id;

  string sql = "select AGENT_NAME_ID from BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE;";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        id = sqlite_wrapper_->ColumnInt64(statement, 0);
        ids.push_back(id);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetAgentsIdsWithConfiguration: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return ids;
}

void RawDatabaseFunctions::AddDailyUserStatistic(const ::bash::database::detail::entity::DailyUserStatistic &us) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::AddDailyUserStatistic: Function call";

  string sql = "insert into BASH_DAILY_USER_STATISTICS_TABLE (AGENT_NAME_ID, USER_ID, DATE_ID, CLASSIFICATION) "
      " values ("
      + to_string(us.agent_name_id) + ","
      + to_string(us.user_id) + ","
      + to_string(us.date_id) + ","
      + to_string(static_cast<int> (us.classification)) +
      " );";

  sqlite_wrapper_->Exec(sql);
}

::database::type::RowId RawDatabaseFunctions::GetDailyUserStatisticId(::database::type::RowId agent_name_id,
                                                                      ::database::type::RowId user_id,
                                                                      ::database::type::RowId date_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetDailyUserStatisticId: Function call";

  string sql = "select ID from BASH_DAILY_USER_STATISTICS_TABLE "
      " where AGENT_NAME_ID= " + to_string(agent_name_id) +
      "  and USER_ID= " + to_string(user_id) +
      "  and DATE_ID= " + to_string(date_id) +
      ";";

  return sqlite_wrapper_->GetFirstInt64Column(sql);
}

void RawDatabaseFunctions::SetDailyUserStatisticsClassification(const ::database::type::RowIds &ids, ::database::type::Classification classification) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetDailyUserStatisticId: Function call";

  string sql =
      "update BASH_DAILY_USER_STATISTICS_TABLE "
      " set CLASSIFICATION= " + to_string(static_cast<int> (classification)) +
      " where ID in (";

  auto end = ids.end();
  for (auto it = ids.begin(); it != end; ++it) {
    sql += to_string(*it);

    if (it != end - 1)
      sql += ", ";
  }

  sql += ");";

  sqlite_wrapper_->Exec(sql);
}

void RawDatabaseFunctions::AddDailyUserCommandStatistic(const ::bash::database::detail::entity::DailyUserCommandStatistic &ucs) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetDailyUserStatisticId: Function call";

  string sql = "insert into BASH_DAILY_USER_COMMAND_STATISTICS_TABLE (STATISTIC_ID, COMMAND_ID, SUMMARY) "
      " values ("
      + to_string(ucs.daily_user_statistic_id) + ","
      + to_string(ucs.command_id) + ","
      + to_string(ucs.summary) +
      " );";

  sqlite_wrapper_->Exec(sql);
}

::database::entity::AgentNames RawDatabaseFunctions::GetAgentsWithExistingDailyUserStatistics() {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetAgentsWithExistingDailyUserStatistics: Function call";

  ::database::entity::AgentNames names;
  ::database::entity::AgentName name;

  const char *sql =
      "select distinct AN.ID, AN.AGENT_NAME from BASH_DAILY_USER_STATISTICS_TABLE as BDUST "
      " left join AGENT_NAMES as AN "
      " on BDUST.AGENT_NAME_ID=AN.ID;";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        name.id = sqlite_wrapper_->ColumnInt64(statement, 0);
        name.agent_name = sqlite_wrapper_->ColumnText(statement, 1);

        names.push_back(name);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetAgentsWithExistingDailyUserStatistics: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return names;
}

entity::AnomalyDetectionConfigurations RawDatabaseFunctions::GetAnomalyDetectionConfigurations() {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetAnomalyDetectionConfiguration: Function call";

  entity::AnomalyDetectionConfigurations configurations;
  entity::AnomalyDetectionConfiguration config;

  const char *sql = "select ID, AGENT_NAME_ID, BEGIN_DATE_ID, END_DATE_ID, CHANGED from BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE;";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        config.id = sqlite_wrapper_->ColumnInt64(statement, 0);
        config.agent_name_id = sqlite_wrapper_->ColumnInt64(statement, 1);
        config.begin_date_id = sqlite_wrapper_->ColumnInt64(statement, 2);
        config.end_date_id = sqlite_wrapper_->ColumnInt64(statement, 3);
        config.changed = static_cast<bool> (sqlite_wrapper_->ColumnInt(statement, 4));

        configurations.push_back(config);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetAgentIdsWithoutConfiguration: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return configurations;
}

void RawDatabaseFunctions::RemoveAnomalyDetectionConfiguration(::database::type::RowId id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::RemoveAnomalyDetectionConfiguration: Function call";

  string sql = "delete from BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE where ID=" + to_string(id);

  sqlite_wrapper_->Exec(sql);
}

void RawDatabaseFunctions::AddAnomalyDetectionConfiguration(const entity::AnomalyDetectionConfiguration &configuration) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::AddAnomalyDetectionConfiguration: Function call";

  string sql = "insert or replace into BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE (ID, AGENT_NAME_ID, BEGIN_DATE_ID, END_DATE_ID, CHANGED) values ("
      "(select ID from BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE where AGENT_NAME_ID=" + to_string(configuration.agent_name_id) + "), " +
      to_string(configuration.agent_name_id) + ", " +
      to_string(configuration.begin_date_id) + ", " +
      to_string(configuration.end_date_id) + ", " +
      to_string(configuration.changed) +
      ");";

  sqlite_wrapper_->Exec(sql);
}

::database::type::RowId RawDatabaseFunctions::GetConfigurationIdForAgent(::database::type::RowId agent_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetConfigurationIdForAgent: Function call";

  string sql = "select ID from BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE where AGENT_NAME_ID=" + to_string(agent_id) + ";";

  return sqlite_wrapper_->GetFirstInt64Column(sql);
}

void RawDatabaseFunctions::AddDefaultCommandsToConfiguration(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::AddDefaultCommandsToConfiguration: Function call";

  string sql =
      "insert into BASH_SELECTED_COMMANDS_TABLE (CONFIGURATION_ID, COMMAND_ID) "
      "  select BADCT.ID, COMMAND_ID "
      "  from BASH_DATE_RANGE_COMMANDS_STATISTICS_TABLE as BDRCST join BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE as BADCT "
      "  on BDRCST.AGENT_NAME_ID=BADCT.AGENT_NAME_ID and BDRCST.BEGIN_DATE_ID=BADCT.BEGIN_DATE_ID and BDRCST.END_DATE_ID=BADCT.END_DATE_ID "
      "  where BADCT.ID=" + to_string(configuration_id) + " order by SUMMARY desc limit 100;";

  sqlite_wrapper_->Exec(sql);
}

void RawDatabaseFunctions::MarkConfigurationAsUnchanged(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::MarkConfigurationAsUnchanged: Function call";

  string sql =
      "update BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE "
      " set CHANGED=0 "
      " where ID=" + to_string(configuration_id) +
      ";";

  sqlite_wrapper_->Exec(sql);
}

void RawDatabaseFunctions::MarkConfigurationAsChanged(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::MarkConfigurationAsUnchanged: Function call";

  string sql =
      "update BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE "
      " set CHANGED=1 "
      " where ID=" + to_string(configuration_id) +
      ";";

  sqlite_wrapper_->Exec(sql);
}

void RawDatabaseFunctions::AddCommandStatistic(const entity::CommandStatistic &statistic) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::AddCommandStatistic: Function call";

  string sql = "insert or ignore into BASH_DATE_RANGE_COMMANDS_STATISTICS_TABLE ( "
      " AGENT_NAME_ID, "
      " COMMAND_ID, "
      " BEGIN_DATE_ID, "
      " END_DATE_ID, "
      " SUMMARY "
      ") "
      "values ( " +
      to_string(statistic.agent_name_id) + ", " +
      to_string(statistic.command_id) + ", " +
      to_string(statistic.begin_date_id) + ", " +
      to_string(statistic.end_date_id) + ", " +
      to_string(statistic.summary) +
      ");";

  sqlite_wrapper_->Exec(sql);
}

bool RawDatabaseFunctions::IsCommandStatisticExist(::database::type::RowId agent_name_id,
                                                   ::database::type::RowId command_id,
                                                   ::database::type::RowId begin_date_id,
                                                   ::database::type::RowId end_date_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::IsCommandStatisticExist: Function call";

  string sql = "select count(*) from BASH_DATE_RANGE_COMMANDS_STATISTICS_TABLE "
      " where "
      "  AGENT_NAME_ID=" + to_string(agent_name_id) +
      " and "
      "  COMMAND_ID=" + to_string(command_id) +
      " and "
      "  BEGIN_DATE_ID=" + to_string(begin_date_id) +
      " and "
      "  END_DATE_ID=" + to_string(end_date_id) +
      ";";

  return static_cast<bool> (sqlite_wrapper_->GetFirstInt64Column(sql));
}

entity::CommandsStatistics RawDatabaseFunctions::GetCommandsStatistics(::database::type::RowId agent_name_id,
                                                                       ::database::type::RowId begin_date_id,
                                                                       ::database::type::RowId end_date_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetCommandStatistic: Function call";

  entity::CommandsStatistics statistics;

  entity::CommandStatistic stat;
  stat.agent_name_id = agent_name_id;
  stat.begin_date_id = begin_date_id;
  stat.end_date_id = end_date_id;

  string sql = "select ID, SUMMARY, COMMAND_ID from BASH_DATE_RANGE_COMMANDS_STATISTICS_TABLE "
      " where "
      "  AGENT_NAME_ID=" + to_string(agent_name_id) +
      " and "
      "  BEGIN_DATE_ID=" + to_string(begin_date_id) +
      " and "
      "  END_DATE_ID=" + to_string(end_date_id) +
      ";";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        stat.id = sqlite_wrapper_->ColumnInt64(statement, 0);
        stat.summary = sqlite_wrapper_->ColumnInt64(statement, 1);
        stat.command_id = sqlite_wrapper_->ColumnInt64(statement, 2);

        statistics.push_back(stat);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(error) << "bash::database::detail::RawDatabaseFunctions::GetCommandStatistic: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return statistics;
}

entity::CommandsStatistics RawDatabaseFunctions::GetCommandsStatistics(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetCommandStatistic: Function call";

  entity::CommandsStatistics statistics;
  entity::CommandStatistic stat;

  string sql =
      "select BDRCST.ID, BDRCST.SUMMARY, BDRCST.COMMAND_ID, "
      "       BDRCST.AGENT_NAME_ID , BDRCST.BEGIN_DATE_ID , BDRCST.END_DATE_ID"
      " from BASH_DATE_RANGE_COMMANDS_STATISTICS_TABLE as BDRCST "
      " join BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE as BADCT "
      " on BDRCST.AGENT_NAME_ID=BADCT.AGENT_NAME_ID "
      "   and BDRCST.BEGIN_DATE_ID=BADCT.BEGIN_DATE_ID "
      "   and BDRCST.END_DATE_ID=BADCT.END_DATE_ID "
      " where BADCT.ID=" + to_string(configuration_id) + ";";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        stat.id = sqlite_wrapper_->ColumnInt64(statement, 0);
        stat.summary = sqlite_wrapper_->ColumnInt64(statement, 1);
        stat.command_id = sqlite_wrapper_->ColumnInt64(statement, 2);
        stat.agent_name_id = sqlite_wrapper_->ColumnInt64(statement, 3);
        stat.begin_date_id = sqlite_wrapper_->ColumnInt64(statement, 4);
        stat.end_date_id = sqlite_wrapper_->ColumnInt64(statement, 5);

        statistics.push_back(stat);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(error) << "bash::database::detail::RawDatabaseFunctions::GetCommandStatistic: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return statistics;
}

::database::type::RowIds RawDatabaseFunctions::GetMarkedCommandsIds(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetMarkedCommandsIds: Function call";

  ::database::type::RowIds ids;

  string sql = "select COMMAND_ID from BASH_SELECTED_COMMANDS_TABLE "
      " where CONFIGURATION_ID=" + to_string(configuration_id) + " order by COMMAND_ID;";

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        auto id = sqlite_wrapper_->ColumnInt64(statement, 0);

        ids.push_back(id);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetMarkedCommandsIds: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return ids;
}

void RawDatabaseFunctions::AddSelectedCommandsIds(::database::type::RowId configuration_id,
                                                  ::database::type::RowIds command_names_ids) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::AddSelectedCommandsIds: Function call";

  string sql = "begin transaction; ";

  for (const auto &id : command_names_ids)
    sql +=
      "insert into BASH_SELECTED_COMMANDS_TABLE ( CONFIGURATION_ID, COMMAND_ID ) "
      "  values ( " + to_string(configuration_id) + ", " + to_string(id) + " ); ";

  sql += " end transaction; ";

  sqlite_wrapper_->Exec(sql);
}

::database::type::RowsCount RawDatabaseFunctions::CommandSummary(::database::type::RowId command_id, ::database::type::RowIds date_range_ids) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::CommandSummary: Function call";

  string sql = "select sum(SUMMARY) from BASH_DAILY_STATISTICS_TABLE "
      " where "
      "  DATE_ID in (";

  auto end = date_range_ids.end();
  for (auto it = date_range_ids.begin(); it != end; ++it) {
    sql += to_string(*it);

    if (it != end - 1)
      sql += ", ";
  }

  sql += ") and COMMAND_ID=" + to_string(command_id) + ";";

  return sqlite_wrapper_->GetFirstInt64Column(sql, 0);
}

void RawDatabaseFunctions::RemoveAllCommandsFromConfiguration(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::RemoveAllCommandsFromConfiguration: Function call";

  string sql = "delete from BASH_SELECTED_COMMANDS_TABLE where CONFIGURATION_ID=" + to_string(configuration_id) + ";";

  sqlite_wrapper_->Exec(sql);
}

::database::type::RowIds RawDatabaseFunctions::GetNotCalculatedDatesIdsFromLogs(::database::type::RowId agent_name_id,
                                                                                ::database::type::RowId user_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::RemoveAllCommandsFromConfiguration: Function call";

  string sql = "select distinct BASH_LOGS_TABLE.DATE_ID from BASH_LOGS_TABLE "
      " left join BASH_DAILY_USER_STATISTICS_TABLE "
      " on BASH_LOGS_TABLE.DATE_ID=BASH_DAILY_USER_STATISTICS_TABLE.DATE_ID and BASH_LOGS_TABLE.AGENT_NAME_ID=BASH_DAILY_USER_STATISTICS_TABLE.AGENT_NAME_ID "
      " where BASH_DAILY_USER_STATISTICS_TABLE.DATE_ID is null"
      " and BASH_LOGS_TABLE.AGENT_NAME_ID=" + to_string(agent_name_id) +
      " and BASH_LOGS_TABLE.USER_ID=" + to_string(user_id) +
      ";";

  ::database::type::RowIds ids;
  ::database::type::RowId id;

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        id = sqlite_wrapper_->ColumnInt64(statement, 0);
        ids.push_back(id);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetNotCalculatedDatesIdsFromLogs: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return ids;
}

::database::type::RowIds RawDatabaseFunctions::GetCommandsIdsFromLogs(::database::type::RowId agent_name_id,
                                                                      ::database::type::RowId user_id,
                                                                      ::database::type::RowId date_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetCommandsIdsFromLogs: Function call";

  string sql = "select distinct COMMAND_ID from BASH_LOGS_TABLE "
      " where AGENT_NAME_ID=" + to_string(agent_name_id) +
      " and DATE_ID=" + to_string(date_id) +
      " and USER_ID=" + to_string(user_id) +
      ";";

  ::database::type::RowIds ids;
  ::database::type::RowId id;

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        id = sqlite_wrapper_->ColumnInt64(statement, 0);
        ids.push_back(id);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetCommandsIdsFromLogs: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return ids;
}

void RawDatabaseFunctions::AddDailyUserStatisticsToConfiguration(::database::type::RowId configuration_id,
                                                                 const ::database::type::RowIds &date_range_ids) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::AddDailyUserStatisticsToConfiguration: Function call";

  string sql =
      "insert or ignore into BASH_ANOMALY_DETECTION_CONFIGURATION_SELECTED_STATISTICS_TABLE (CONFIGURATION_ID, STATISTIC_ID)"
      " select " + to_string(configuration_id) +
      ", ID from BASH_DAILY_USER_STATISTICS_TABLE "
      "  where DATE_ID in (";

  auto end = date_range_ids.end();
  for (auto it = date_range_ids.begin(); it != end; ++it) {
    sql += to_string(*it);

    if (it != end - 1)
      sql += ", ";
  }

  sql += ");";

  sqlite_wrapper_->Exec(sql);
}

void RawDatabaseFunctions::RemoveDailyStatisticsFromConfiguration(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::RemoveDailyStatisticsFromConfiguration: Function call";

  string sql = "delete from BASH_ANOMALY_DETECTION_CONFIGURATION_SELECTED_STATISTICS_TABLE "
      " where CONFIGURATION_ID=" + to_string(configuration_id) + ";";

  sqlite_wrapper_->Exec(sql);
}

::bash::database::detail::entity::DailyUserStatistics RawDatabaseFunctions::GetDailyUserStatisticsFromConfiguration(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::AddDailyUserStatisticsToConfiguration: Function call";

  string sql =
      "select BDUST.ID, BDUST.AGENT_NAME_ID, BDUST.USER_ID, BDUST.DATE_ID, BDUST.CLASSIFICATION "
      " from BASH_DAILY_USER_STATISTICS_TABLE as BDUST "
      " join BASH_ANOMALY_DETECTION_CONFIGURATION_SELECTED_STATISTICS_TABLE as BADCSST "
      " on BDUST.ID=BADCSST.STATISTIC_ID "
      " where BADCSST.CONFIGURATION_ID=" + to_string(configuration_id) + ";";

  ::bash::database::detail::entity::DailyUserStatistics statistics;
  ::bash::database::detail::entity::DailyUserStatistic stat;

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        stat.id = sqlite_wrapper_->ColumnInt64(statement, 0);
        stat.agent_name_id = sqlite_wrapper_->ColumnInt64(statement, 1);
        stat.user_id = sqlite_wrapper_->ColumnInt64(statement, 2);
        stat.date_id = sqlite_wrapper_->ColumnInt64(statement, 3);
        stat.classification = static_cast< ::database::type::Classification> (sqlite_wrapper_->ColumnInt(statement, 4));

        statistics.push_back(stat);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetCommandsIdsFromLogs: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return statistics;
}

::bash::database::detail::entity::DailyUserStatistics RawDatabaseFunctions::GetDailyUserStatisticsForAgent(::database::type::RowId agent_name_id,
                                                                                                           const ::database::type::RowIds &date_range_ids) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetDailyUserStatisticsForAgent: Function call";

  string sql =
      "select BDUST.ID, BDUST.USER_ID, BDUST.DATE_ID, BDUST.CLASSIFICATION "
      " from BASH_DAILY_USER_STATISTICS_TABLE as BDUST "
      " where BDUST.AGENT_NAME_ID=" + to_string(agent_name_id) +
      " and BDUST.DATE_ID in (";

  auto end = date_range_ids.end();
  for (auto it = date_range_ids.begin(); it != end; ++it) {
    sql += to_string(*it);

    if (it != end - 1)
      sql += ", ";
  }

  sql += ");";

  ::bash::database::detail::entity::DailyUserStatistics statistics;
  ::bash::database::detail::entity::DailyUserStatistic stat;
  stat.agent_name_id = agent_name_id;

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        stat.id = sqlite_wrapper_->ColumnInt64(statement, 0);
        stat.user_id = sqlite_wrapper_->ColumnInt64(statement, 1);
        stat.date_id = sqlite_wrapper_->ColumnInt64(statement, 2);
        stat.classification = static_cast< ::database::type::Classification> (sqlite_wrapper_->ColumnInt(statement, 3));

        statistics.push_back(stat);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetCommandsIdsFromLogs: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return statistics;
}

::database::type::RowIds RawDatabaseFunctions::GetUsersIdsFromSelectedDailyStatisticsInConfiguration(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetUsersIdsFromDailyStatistics: Function call";

  string sql =
      "select distinct BDUST.USER_ID from BASH_ANOMALY_DETECTION_CONFIGURATION_SELECTED_STATISTICS_TABLE as BADCSST "
      " left join BASH_DAILY_USER_STATISTICS_TABLE as BDUST "
      " on BADCSST.STATISTIC_ID=BDUST.ID "
      " where BADCSST.CONFIGURATION_ID=" + to_string(configuration_id) +
      ";";

  ::database::type::RowIds ids;
  ::database::type::RowId id;

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        id = sqlite_wrapper_->ColumnInt64(statement, 0);
        ids.push_back(id);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetUsersIdsFromDailyStatistics: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return ids;
}

::database::type::RowId RawDatabaseFunctions::CountSelectedDailyStatisticsWithoutUnknownClassificationInConfiguration(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::CountSelectedDailyStatisticsWithoutUnknownClassificationInConfiguration: Function call";

  string sql =
      "select count(*) from BASH_ANOMALY_DETECTION_CONFIGURATION_SELECTED_STATISTICS_TABLE as BADCSST "
      " left join BASH_DAILY_USER_STATISTICS_TABLE as BDUST on BADCSST.STATISTIC_ID=BDUST.ID "
      " where BADCSST.CONFIGURATION_ID=" + to_string(configuration_id) +
      "   and BDUST.CLASSIFICATION != " + to_string(static_cast<int> (::database::type::Classification::UNKNOWN)) +
      ";";

  return sqlite_wrapper_->GetFirstInt64Column(sql);
}

::database::type::RowsCount RawDatabaseFunctions::CountSelectedDailyUserStatisticsWithoutUnknownClassificationFromConfigurationByUser(::database::type::RowId configuration_id,
                                                                                                                                      ::database::type::RowId user_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::CountSelectedDailyUserStatisticsWithoutUnknownClassificationFromConfigurationByUser: Function call";

  string sql =
      "select count(*) from BASH_ANOMALY_DETECTION_CONFIGURATION_SELECTED_STATISTICS_TABLE as BADCSST "
      " left join BASH_DAILY_USER_STATISTICS_TABLE as BDUST on BADCSST.STATISTIC_ID=BDUST.ID "
      " where BADCSST.CONFIGURATION_ID=" + to_string(configuration_id) +
      "   and BDUST.USER_ID=" + to_string(user_id) +
      "   and BDUST.CLASSIFICATION != " + to_string(static_cast<int> (::database::type::Classification::UNKNOWN)) +
      ";";

  return sqlite_wrapper_->GetFirstInt64Column(sql);

}

::bash::database::detail::entity::DailyUserStatistics RawDatabaseFunctions::GetSelectedDailyUserStatisticsWithoutUnknownClassificationFromConfigurationByUser(::database::type::RowId configuration_id,
                                                                                                                                                              ::database::type::RowId user_id,
                                                                                                                                                              ::database::type::RowsCount limit,
                                                                                                                                                              ::database::type::RowsCount offset) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetSelectedDailyUserStatisticsWithoutUnknownClassificationFromConfigurationByUser: Function call";

  string sql =
      "select BDUST.ID, BDUST.AGENT_NAME_ID, BDUST.DATE_ID, BDUST.CLASSIFICATION from BASH_DAILY_USER_STATISTICS_TABLE as BDUST "
      " where BDUST.ID in ("
      "   select BADCSST.STATISTIC_ID from BASH_ANOMALY_DETECTION_CONFIGURATION_SELECTED_STATISTICS_TABLE as BADCSST "
      "    left join BASH_ANOMALY_DETECTION_CONFIGURATION_TABLE as BADCT "
      "    on BADCSST.CONFIGURATION_ID=BADCT.ID "
      "    where BADCSST.CONFIGURATION_ID=" + to_string(configuration_id) +
      " )"
      " and BDUST.USER_ID=" + to_string(user_id) +
      " and BDUST.CLASSIFICATION != " + to_string(static_cast<int> (::database::type::Classification::UNKNOWN)) +
      " limit " + to_string(limit) +
      " offset " + to_string(offset) +
      ";";

  ::bash::database::detail::entity::DailyUserStatistics statistics;
  ::bash::database::detail::entity::DailyUserStatistic stat;
  stat.user_id = user_id;

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        stat.id = sqlite_wrapper_->ColumnInt64(statement, 0);
        stat.agent_name_id = sqlite_wrapper_->ColumnInt64(statement, 1);
        stat.date_id = sqlite_wrapper_->ColumnInt64(statement, 2);
        stat.classification = static_cast< ::database::type::Classification> (sqlite_wrapper_->ColumnInt(statement, 3));

        statistics.push_back(stat);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetCommandsIdsFromLogs: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return statistics;
}

::bash::database::detail::entity::DailyUserCommandsStatistics RawDatabaseFunctions::GetSelectedDailyUserCommandsStatistics(::database::type::RowId statistic_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetSelectedDailyUserCommandsStatistics: Function call";

  string sql =
      "select BDUCST.ID, BDUCST.STATISTIC_ID, BDUCST.COMMAND_ID, BDUCST.SUMMARY from BASH_SELECTED_COMMANDS_TABLE as BSCT "
      " left join BASH_DAILY_USER_COMMAND_STATISTICS_TABLE as BDUCST "
      " on BSCT.COMMAND_ID=BDUCST.COMMAND_ID "
      " where BDUCST.STATISTIC_ID=" + to_string(statistic_id) +
      " order by BDUCST.COMMAND_ID"
      ";";

  ::bash::database::detail::entity::DailyUserCommandsStatistics statistics;
  ::bash::database::detail::entity::DailyUserCommandStatistic stat;

  sqlite3_stmt *statement = nullptr;
  sqlite_wrapper_->Prepare(sql, &statement);

  try {
    do {
      auto ret = sqlite_wrapper_->Step(statement);

      if (ret == SQLITE_ROW) {
        stat.id = sqlite_wrapper_->ColumnInt64(statement, 0);
        stat.daily_user_statistic_id = sqlite_wrapper_->ColumnInt64(statement, 1);
        stat.command_id = sqlite_wrapper_->ColumnInt64(statement, 2);
        stat.summary = sqlite_wrapper_->ColumnInt64(statement, 3);

        statistics.push_back(stat);
      }
      else
        break;
    }
    while (true);
  }
  catch (::database::exception::DatabaseException &ex) {
    BOOST_LOG_TRIVIAL(debug) << "bash::database::detail::RawDatabaseFunctions::GetSelectedDailyUserCommandsStatistics: Exception catched: " << ex.what();
    sqlite_wrapper_->Finalize(statement);
    throw;
  }

  sqlite_wrapper_->Finalize(statement);

  return statistics;
}

RawDatabaseFunctions::RawDatabaseFunctions(::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper) :
sqlite_wrapper_(sqlite_wrapper) {
}

}

}

}
