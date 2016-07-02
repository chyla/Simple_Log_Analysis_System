/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "scripts.h"

#include <boost/log/trivial.hpp>

namespace bash
{

namespace domain
{

ScriptsPtr Scripts::Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                           ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) {
  return ScriptsPtr(new Scripts(database_functions, general_database_functions));
}

void Scripts::AddLog(const ::type::BashLogEntry &log_entry) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::AddLog: Function call";

  general_database_functions_->AddDate(log_entry.utc_time.GetDate());
  general_database_functions_->AddTime(log_entry.utc_time.GetTime());
  general_database_functions_->AddAgentName(log_entry.agent_name);
  database_functions_->AddSystemUser(log_entry.user_id);
  database_functions_->AddCommand(log_entry.command);

  database_functions_->AddLog(log_entry);
}

Scripts::Scripts(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                 ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) :
database_functions_(database_functions),
general_database_functions_(general_database_functions) {
}

}

}
