/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_APACHE_DATABASE_DATABASE_FUNCTIONS_H
#define SRC_APACHE_DATABASE_DATABASE_FUNCTIONS_H

#include <memory>

#include "detail/database_functions_interface.h"

#include "src/database/detail/general_database_functions_interface.h"
#include "src/database/detail/sqlite_wrapper_interface.h"
// will be removed in the future
#include "src/database/database.h"

namespace apache
{

namespace database
{

class DatabaseFunctions;
typedef std::shared_ptr<DatabaseFunctions> DatabaseFunctionsPtr;

class DatabaseFunctions : public detail::DatabaseFunctionsInterface {
 public:
  virtual ~DatabaseFunctions() = default;

  static DatabaseFunctionsPtr Create(::database::DatabasePtr db,
                                     ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper,
                                     ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);

  void CreateTables() override;

  ::database::type::RowsCount GetLogsCount(std::string agent_name, std::string virtualhost_name,
                                           ::type::Timestamp from, ::type::Timestamp to) override;

  ::type::ApacheLogs GetLogs(std::string agent_name, std::string virtualhost_name,
                             ::type::Timestamp from, ::type::Timestamp to,
                             unsigned limit, ::database::type::RowsCount offset) override;

  bool AddSessionStatistics(const ::apache::type::ApacheSessions &sessions) override;

  void MarkStatisticsAsCreatedFor(::type::Date date) override;

  bool AreStatisticsCreatedFor(::type::Date date) override;

  virtual ::database::type::AgentNames GetAgentNames() override;

  virtual ::database::type::VirtualhostNames GetVirtualhostNames(::database::type::AgentName agent_name) override;

  bool IsLastRunSet(const ::apache::type::LastRunType &type) override;

  void SetLastRun(const ::apache::type::LastRunType &type, const ::type::Timestamp &date) override;

  ::type::Timestamp GetLastRun(const ::apache::type::LastRunType &type) override;

  void AddVirtualhostName(const std::string &name) override;
  ::database::type::RowId AddAndGetVirtualhostNameId(const std::string &name) override;
  ::database::type::RowId GetVirtualhostNameId(const std::string &name) override;
  std::string GetVirtualhostNameById(const ::database::type::RowId &id) override;

 private:
  ::database::DatabasePtr db_;
  ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper_;
  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;

  DatabaseFunctions(::database::DatabasePtr db,
                    ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper,
                    ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);
};

}

}

#endif /* SRC_APACHE_DATABASE_DATABASE_FUNCTIONS_H */
