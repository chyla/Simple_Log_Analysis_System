/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "database_functions.h"

using ::type::Timestamp;
using ::type::Time;
using ::type::Date;

namespace apache
{

namespace database
{

DatabaseFunctionsPtr DatabaseFunctions::Create(::database::DatabasePtr db,
                                               ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper) {
  return DatabaseFunctionsPtr(new DatabaseFunctions(db, sqlite_wrapper));
}

::database::type::RowsCount DatabaseFunctions::GetApacheLogsCount(std::string agent_name, std::string virtualhost_name,
                                                                  ::type::Date from, ::type::Date to) {
  return db_->GetApacheLogsCount(agent_name, virtualhost_name,
                                 Timestamp::Create(Time::Create(0, 0, 0), from),
                                 Timestamp::Create(Time::Create(23, 59, 59), to));
}

::type::ApacheLogs DatabaseFunctions::GetApacheLogs(std::string agent_name, std::string virtualhost_name,
                                                    ::type::Date from, ::type::Date to,
                                                    unsigned limit, ::database::type::RowsCount offset) {
  return db_->GetApacheLogs(agent_name, virtualhost_name,
                            Timestamp::Create(Time::Create(0, 0, 0), from),
                            Timestamp::Create(Time::Create(23, 59, 59), to),
                            limit, offset);
}

bool DatabaseFunctions::AddApacheSessionStatistics(const ::apache::type::ApacheSessions &sessions) {
  return db_->AddApacheSessionStatistics(sessions);
}

void DatabaseFunctions::MarkApacheStatisticsAsCreatedFor(::type::Date date) {
  return db_->MarkApacheStatisticsAsCreatedFor(date.GetDay(), date.GetMonth(), date.GetYear());
}

bool DatabaseFunctions::IsApacheStatisticsCreatedFor(::type::Date date) {
  return db_->IsApacheStatisticsCreatedFor(date.GetDay(), date.GetMonth(), date.GetYear());
}

DatabaseFunctions::DatabaseFunctions(::database::DatabasePtr db,
                                     ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper) :
db_(db),
sqlite_wrapper_(sqlite_wrapper) {
}

}

}
