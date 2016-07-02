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
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::Create: Function call";

  raw_database_functions_->CreateTables();
}

void DatabaseFunctions::AddSystemUser(type::UID uid) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::AddVirtualhostName: Function call";

  raw_database_functions_->AddSystemUser({uid});
}

::database::type::RowId DatabaseFunctions::GetSystemUserId(type::UID uid) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::GetSystemUserId: Function call";

  return raw_database_functions_->GetSystemUserId({uid});
}

void DatabaseFunctions::AddLog(const ::type::BashLogEntry &log_entry) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::AddLog: Function call";

  detail::entity::Log raw_log;
  raw_log.agent_name_id = general_database_functions_->GetAgentNameId(log_entry.agent_name);
  raw_log.date_id = general_database_functions_->GetDateId(log_entry.utc_time.GetDate());
  raw_log.time_id = general_database_functions_->GetTimeId(log_entry.utc_time.GetTime());
  raw_log.user_id = GetSystemUserId(log_entry.user_id);
  raw_log.command = log_entry.command;

  raw_database_functions_->AddLog(raw_log);
}

DatabaseFunctions::DatabaseFunctions(::bash::database::detail::RawDatabaseFunctionsInterfacePtr raw_database_functions,
                                     ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) :
raw_database_functions_(raw_database_functions),
general_database_functions_(general_database_functions) {
}

}

}
