/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "database_functions.h"

#include <boost/log/trivial.hpp>
#include <string>

#include "type/uid.h"
#include "detail/raw_database_functions.h"

using namespace std;

namespace bash
{

namespace database
{

DatabaseFunctionsPtr DatabaseFunctions::Create(::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper,
                                               ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::Create: Function call";

  auto raw_database_functions = detail::RawDatabaseFunctions::Create(sqlite_wrapper);

  return Create(raw_database_functions, general_database_functions);
}

DatabaseFunctionsPtr DatabaseFunctions::Create(::bash::database::detail::RawDatabaseFunctionsInterfacePtr raw_database_functions,
                                               ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::Create: Function call";

  return DatabaseFunctionsPtr(new DatabaseFunctions(raw_database_functions, general_database_functions));
}

void DatabaseFunctions::CreateTables() {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::CreateTables: Function call";

  raw_database_functions_->CreateTables();
}

void DatabaseFunctions::AddSystemUser(type::UID uid) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::AddSystemUser: Function call";

  raw_database_functions_->AddSystemUser({uid});
}

::database::type::RowId DatabaseFunctions::GetSystemUserId(type::UID uid) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::GetSystemUserId: Function call";

  return raw_database_functions_->GetSystemUserId({uid});
}

void DatabaseFunctions::AddCommand(const ::bash::database::type::CommandName &command) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::AddCommand: Function call";

  return raw_database_functions_->AddCommand(command);
}

::database::type::RowId DatabaseFunctions::GetCommandId(const ::bash::database::type::CommandName &command) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetCommandId: Function call";

  return raw_database_functions_->GetCommandId(command);
}

::database::type::RowIds DatabaseFunctions::GetAllCommandsIds() {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetAllCommandsIds: Function call";

  return raw_database_functions_->GetAllCommandsIds();
}

void DatabaseFunctions::AddLog(const ::type::BashLogEntry &log_entry) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::AddLog: Function call";

  detail::entity::Log raw_log;
  raw_log.agent_name_id = general_database_functions_->GetAgentNameId(log_entry.agent_name);
  raw_log.date_id = general_database_functions_->GetDateId(log_entry.utc_time.GetDate());
  raw_log.time_id = general_database_functions_->GetTimeId(log_entry.utc_time.GetTime());
  raw_log.user_id = GetSystemUserId(log_entry.user_id);
  raw_log.command_id = raw_database_functions_->GetCommandId(log_entry.command);

  raw_database_functions_->AddLog(raw_log);
}

::database::type::RowsCount DatabaseFunctions::CountCommandsForDailySystemStatistic(::database::type::RowId agent_name_id,
                                                                                    ::database::type::RowId date_id,
                                                                                    ::database::type::RowId command_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::CountCommandsForDailySystemStatistic: Function call";

  return raw_database_functions_->CountCommandsForDailySystemStatistic(agent_name_id, date_id, command_id);
}

void DatabaseFunctions::AddDailySystemStatistic(const detail::entity::DailySystemStatistic &statistics) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::AddDailySystemStatistic: Function call";

  raw_database_functions_->AddDailySystemStatistic(statistics);
}

::database::type::RowIds DatabaseFunctions::GetDateIdsWithoutCreatedDailySystemStatistic(::database::type::RowId agent_name_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetDateIdsWithoutCreatedDailySystemStatistic: Function call";

  return raw_database_functions_->GetDateIdsWithoutCreatedDailySystemStatistic(agent_name_id);
}

DatabaseFunctions::DatabaseFunctions(::bash::database::detail::RawDatabaseFunctionsInterfacePtr raw_database_functions,
                                     ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) :
raw_database_functions_(raw_database_functions),
general_database_functions_(general_database_functions) {
}

}

}
