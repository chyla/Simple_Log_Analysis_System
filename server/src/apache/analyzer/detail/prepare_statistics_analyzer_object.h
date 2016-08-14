/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include <memory>
#include <utility>
#include <map>

#include <slas/type/apache_log_entry.h>

#include "prepare_statistics_analyzer_object_interface.h"
#include "src/apache/database/detail/database_functions_interface.h"
#include "src/database/type/row_id.h"
#include "src/database/type/rows_count.h"
#include "src/apache/type/apache_session_entry.h"


namespace apache
{

namespace analyzer
{

namespace detail
{

class PrepareStatisticsAnalyzerObject;
typedef std::shared_ptr<PrepareStatisticsAnalyzerObject> PrepareStatisticsAnalyzerObjectPtr;

class PrepareStatisticsAnalyzerObject : public PrepareStatisticsAnalyzerObjectInterface {
 public:
  virtual ~PrepareStatisticsAnalyzerObject() = default;

  static PrepareStatisticsAnalyzerObjectPtr Create(::apache::database::detail::DatabaseFunctionsInterfacePtr database_functions);

  void Prepare(const ::type::Timestamp &now) override;

 private:
  PrepareStatisticsAnalyzerObject(::apache::database::detail::DatabaseFunctionsInterfacePtr database_functions);

  void CreateStatistics(const ::database::type::AgentName &agent_name,
                        const ::database::type::VirtualhostName &virtualhost_name);

  void CalculateStatistics(const ::database::type::AgentName &agent_name,
                           const ::database::type::VirtualhostName &virtualhost_name,
                           ::database::type::RowsCount count);

  ::type::Timestamp GetLastStatisticsCalculationTimestamp();

  bool IsErrorCode(const int &status_code) const;
  bool ShouldStatisticsBeCalculated(const ::type::Timestamp &last_statistics_calculation,
                                    const ::type::Timestamp &now);
  bool IsInThisSameSession(const ::apache::type::ApacheSessionEntry &session,
                           const ::type::ApacheLogEntry &log_entry);
  ::apache::type::ApacheSessionEntry CreateSession(const ::type::ApacheLogEntry &log_entry);
  void UpdateCurrentStatisticsCalculationTime(const ::apache::type::ApacheSessionEntry &session);

  void SaveAllSessions();

  typedef std::pair<std::string, std::string> UniqueSessionId;
  ::apache::type::ApacheSessions calculated_sessions_statistics_;

  ::apache::database::detail::DatabaseFunctionsInterfacePtr database_functions_;
  std::map<UniqueSessionId, ::apache::type::ApacheSessionEntry> sessions_statistics_;
};

}

}

}
