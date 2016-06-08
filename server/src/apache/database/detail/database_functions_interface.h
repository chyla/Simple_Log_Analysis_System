/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_APACHE_DATABASE_DETAIL_DATABASE_FUNCTIONS_INTERFACE_H
#define SRC_APACHE_DATABASE_DETAIL_DATABASE_FUNCTIONS_INTERFACE_H

#include <memory>

#include <patlms/type/date.h>
#include <patlms/type/apache_log_entry.h>

#include "src/apache/type/apache_session_entry.h"
#include "src/apache/type/last_run_type.h"
#include "src/database/type/rows_count.h"
#include "src/database/type/agent_name.h"
#include "src/database/type/virtualhost_name.h"
#include "src/apache/type/anomaly_detection_configuration_entry.h"

namespace apache
{

namespace database
{

namespace detail
{

class DatabaseFunctionsInterface {
 public:
  virtual ~DatabaseFunctionsInterface() = default;

  virtual void CreateTables() = 0;

  virtual const ::apache::type::AnomalyDetectionConfiguration GetAnomalyDetectionConfigurations() = 0;

  virtual ::database::type::RowsCount GetLogsCount(std::string agent_name, std::string virtualhost_name,
                                                   ::type::Timestamp from, ::type::Timestamp to) = 0;
  virtual ::type::ApacheLogs GetLogs(std::string agent_name, std::string virtualhost_name,
                                     ::type::Timestamp from, ::type::Timestamp to,
                                     unsigned limit, ::database::type::RowsCount offset) = 0;

  virtual bool AddSessionStatistics(const ::apache::type::ApacheSessions &sessions) = 0;
  virtual ::database::type::RowsCount GetSessionStatisticsCount(const std::string &agent_name, const std::string &virtualhost_name,
                                                                const ::type::Timestamp &from, const ::type::Timestamp &to) = 0;
  virtual ::apache::type::ApacheSessions GetSessionStatistics(const std::string &agent_name, const std::string &virtualhost_name,
                                                              const ::type::Timestamp &from, const ::type::Timestamp &to,
                                                              unsigned limit, ::database::type::RowsCount offset) = 0;
  virtual ::database::type::RowsCount GetSessionStatisticsWithoutLearningSetCount(const std::string &agent_name, const std::string &virtualhost_name,
                                                                                  const ::type::Timestamp &from, const ::type::Timestamp &to) = 0;
  virtual ::apache::type::ApacheSessions GetSessionStatisticsWithoutLearningSet(const std::string &agent_name, const std::string &virtualhost_name,
                                                                                const ::type::Timestamp &from, const ::type::Timestamp &to,
                                                                                unsigned limit, long long offset) = 0;
  virtual ::apache::type::ApacheSessionEntry GetOneSessionStatistic(::database::type::RowId id) = 0;
  virtual void MarkSessionStatisticAsAnomaly(const ::database::type::RowId &id) = 0;

  virtual void MarkStatisticsAsCreatedFor(::type::Date date) = 0;
  virtual bool AreStatisticsCreatedFor(::type::Date date) = 0;

  virtual ::database::type::AgentNames GetAgentNames() = 0;

  virtual ::database::type::VirtualhostNames GetVirtualhostNames(::database::type::AgentName agent_name) = 0;

  virtual bool IsLastRunSet(const ::apache::type::LastRunType &type) = 0;
  virtual void SetLastRun(const ::apache::type::LastRunType &type, const ::type::Timestamp &date) = 0;
  virtual ::type::Timestamp GetLastRun(const ::apache::type::LastRunType &type) = 0;

  virtual void AddVirtualhostName(const std::string &name) = 0;
  virtual ::database::type::RowId AddAndGetVirtualhostNameId(const std::string &name) = 0;
  virtual ::database::type::RowId GetVirtualhostNameId(const std::string &name) = 0;
  virtual std::string GetVirtualhostNameById(const ::database::type::RowId &id) = 0;

  virtual ::database::type::RowIds GetLearningSessionsIds(const ::database::type::RowId &agent_id,
                                                          const ::database::type::RowId &virtualhost_id,
                                                          unsigned limit, ::database::type::RowId offset) = 0;
  virtual ::database::type::RowsCount GetLearningSessionsCount(const ::database::type::RowId &agent_id,
                                                               const ::database::type::RowId &virtualhost_id) = 0;
  virtual void SetLearningSessions(const ::database::type::RowId &agent_id,
                                   const ::database::type::RowId &virtualhost_id,
                                   const ::database::type::RowIds &sessions_ids) = 0;
  virtual void RemoveAllLearningSessions(const ::database::type::RowId &agent_id,
                                         const ::database::type::RowId &virtualhost_id) = 0;
};

typedef std::shared_ptr<DatabaseFunctionsInterface> DatabaseFunctionsInterfacePtr;

}

}

}

#endif /* SRC_APACHE_DATABASE_DETAIL_DATABASE_FUNCTIONS_INTERFACE_H */
