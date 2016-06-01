/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "database_functions.h"

#include <boost/log/trivial.hpp>

using ::type::Timestamp;
using ::type::Time;
using ::type::Date;

namespace apache
{

namespace database
{

DatabaseFunctionsPtr DatabaseFunctions::Create(::database::DatabasePtr db,
                                               ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::Create: Function call";

  return DatabaseFunctionsPtr(new DatabaseFunctions(db, sqlite_wrapper));
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

  return db_->MarkApacheStatisticsAsCreatedFor(date.GetDay(), date.GetMonth(), date.GetYear());
}

bool DatabaseFunctions::IsStatisticsCreatedFor(::type::Date date) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::IsStatisticsCreatedFor: Function call";

  return db_->IsApacheStatisticsCreatedFor(date.GetDay(), date.GetMonth(), date.GetYear());
}

::database::type::AgentNames DatabaseFunctions::GetAgentNames() {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetAgentNames: Function call";

  return db_->GetApacheAgentNames();
}

::database::type::VirtualhostNames DatabaseFunctions::GetVirtualhostNames(::database::type::AgentName agent_name) {
  BOOST_LOG_TRIVIAL(debug) << "apache::database::DatabaseFunctions::GetVirtualhostNames: Function call";

  return db_->GetApacheVirtualhostNames(agent_name);
}

DatabaseFunctions::DatabaseFunctions(::database::DatabasePtr db,
                                     ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper) :
db_(db),
sqlite_wrapper_(sqlite_wrapper) {
}

}

}
