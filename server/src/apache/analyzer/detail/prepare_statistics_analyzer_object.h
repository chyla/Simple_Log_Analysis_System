/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_APACHE_ANALYZER_DETAIL_PREPARE_STATISTICS_ANALYZER_OBJECT_H
#define SRC_APACHE_ANALYZER_DETAIL_PREPARE_STATISTICS_ANALYZER_OBJECT_H

#include <memory>
#include <utility>
#include <map>

#include <patlms/type/apache_log_entry.h>

#include "prepare_statistics_analyzer_object_interface.h"
#include "system_interface.h"
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
  static PrepareStatisticsAnalyzerObjectPtr Create(::apache::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                                   SystemInterfacePtr system_interface);

  void Prepare() override;

 private:
  PrepareStatisticsAnalyzerObject(::apache::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                  SystemInterfacePtr system_interface);

  void CreateStatistics(const ::database::type::AgentName &agent_name,
                        const ::database::type::VirtualhostName &virtualhost_name);

  void CalculateStatistics(const ::database::type::AgentName &agent_name,
                           const ::database::type::VirtualhostName &virtualhost_name,
                           const ::type::Date &from,
                           const ::type::Date &to,
                           ::database::type::RowsCount count,
                           ::database::type::RowId offset);

  ::type::Timestamp GetCurrentTimestamp() const;

  bool IsErrorCode(const int &status_code) const;
  bool IsInThisSameSession(const ::apache::type::ApacheSessionEntry &session,
                           const ::type::ApacheLogEntry &log_entry);

  void SaveAllSessions();
  
  typedef std::pair<std::string, std::string> UniqueSessionId;
  ::apache::type::ApacheSessions calculated_sessions_statistics_;

  ::apache::database::detail::DatabaseFunctionsInterfacePtr database_functions_;
  SystemInterfacePtr system_interface_;
  std::map<UniqueSessionId, ::apache::type::ApacheSessionEntry> sessions_statistics_;
};

}

}

}

#endif /* SRC_APACHE_ANALYZER_DETAIL_PREPARE_STATISTICS_ANALYZER_OBJECT_H */
