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

  const ::apache::type::AnomalyDetectionConfiguration GetAnomalyDetectionConfigurations() override;

  ::database::type::RowsCount GetUnusedLogsCount(std::string agent_name, std::string virtualhost_name) override;
  ::type::ApacheLogs GetUnusedLogs(std::string agent_name, std::string virtualhost_name,
                                   unsigned limit, ::database::type::RowsCount offset) override;
  void MarkLogsAsUsed(const ::type::ApacheLogs &logs) override;

  bool AddSessionStatistics(const ::apache::type::ApacheSessions &sessions) override;
  ::database::type::RowsCount GetSessionStatisticsCount(const std::string &agent_name, const std::string &virtualhost_name,
                                                        const ::type::Timestamp &from, const ::type::Timestamp &to) override;
  ::apache::type::ApacheSessions GetSessionStatistics(const std::string &agent_name, const std::string &virtualhost_name,
                                                      const ::type::Timestamp &from, const ::type::Timestamp &to,
                                                      unsigned limit, ::database::type::RowsCount offset) override;
  ::database::type::RowsCount GetSessionStatisticsWithoutLearningSetCount(const std::string &agent_name, const std::string &virtualhost_name,
                                                                          const ::type::Timestamp &from, const ::type::Timestamp &to) override;
  bool IsSessionStatisticsWithoutLearningSetExists(const std::string &agent_name, const std::string &virtualhost_name) override;
  ::apache::type::ApacheSessions GetSessionStatisticsWithoutLearningSet(const std::string &agent_name, const std::string &virtualhost_name,
                                                                        const ::type::Timestamp &from, const ::type::Timestamp &to,
                                                                        unsigned limit, long long offset) override;
  ::apache::type::ApacheSessionEntry GetOneSessionStatistic(::database::type::RowId id) override;
  void MarkSessionStatisticAsAnomaly(const ::database::type::RowId &id) override;
  void ClearAnomalyMarksInLearningSet(const ::database::type::RowId &agent_name_id,
                                      const ::database::type::RowId &virtualhost_name_id) override;
  void MarkLearningSetWithIqrMethod(const ::database::type::RowId &agent_name_id,
                                    const ::database::type::RowId &virtualhost_name_id) override;

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

  ::database::type::RowIds GetLearningSessionsIds(const ::database::type::RowId &agent_id,
                                                  const ::database::type::RowId &virtualhost_id,
                                                  unsigned limit, ::database::type::RowId offset) override;
  ::database::type::RowsCount GetLearningSessionsCount(const ::database::type::RowId &agent_id,
                                                       const ::database::type::RowId &virtualhost_id) override;
  void SetLearningSessions(const ::database::type::RowId &agent_id,
                           const ::database::type::RowId &virtualhost_id,
                           const ::database::type::RowIds &sessions_ids) override;
  void RemoveAllLearningSessions(const ::database::type::RowId &agent_id,
                                 const ::database::type::RowId &virtualhost_id) override;

 private:
  ::database::DatabasePtr db_;
  ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper_;
  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;

  DatabaseFunctions(::database::DatabasePtr db,
                    ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper,
                    ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);

  std::string GetTimeRule(const ::type::Timestamp &from, const ::type::Timestamp &to) const;
};

}

}

#endif /* SRC_APACHE_DATABASE_DATABASE_FUNCTIONS_H */
