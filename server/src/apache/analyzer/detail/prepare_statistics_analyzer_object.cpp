/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "prepare_statistics_analyzer_object.h"

#include <boost/log/trivial.hpp>
#include <patlms/util/distance.h>

#include "system.h"
#include "session_length.h"

using namespace ::apache::database::detail;
using namespace ::apache::type;
using namespace ::database::type;
using namespace ::type;
using namespace ::util;

namespace apache
{

namespace analyzer
{

namespace detail
{

PrepareStatisticsAnalyzerObjectPtr PrepareStatisticsAnalyzerObject::Create(DatabaseFunctionsInterfacePtr database_functions) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::Create: Function call";
  auto system = System::Create();

  return PrepareStatisticsAnalyzerObjectPtr(new PrepareStatisticsAnalyzerObject(database_functions, system));
}

PrepareStatisticsAnalyzerObjectPtr PrepareStatisticsAnalyzerObject::Create(DatabaseFunctionsInterfacePtr database_functions,
                                                                           SystemInterfacePtr system_interface) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::Create: Function call";

  return PrepareStatisticsAnalyzerObjectPtr(new PrepareStatisticsAnalyzerObject(database_functions, system_interface));
}

void PrepareStatisticsAnalyzerObject::Prepare(const ::type::Timestamp &now) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::Prepare: Function call";

  const auto last_statistics_calculation = GetLastStatisticsCalculationTimestamp();

  if (ShouldStatisticsBeCalculated(last_statistics_calculation, now)) {
    for (auto agent_name : database_functions_->GetAgentNames()) {
      BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::Prepare: Preparing agent: " << agent_name;

      for (auto virtualhost_name : database_functions_->GetVirtualhostNames(agent_name)) {
        BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::Prepare: Preparing virtualhost: " << virtualhost_name;

        CreateStatistics(agent_name, virtualhost_name);
      }
    }
  }
  else {
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::Prepare: Not running, last statistics prepared: " << GetLastStatisticsCalculationTimestamp();
  }
}

PrepareStatisticsAnalyzerObject::PrepareStatisticsAnalyzerObject(DatabaseFunctionsInterfacePtr database_functions,
                                                                 SystemInterfacePtr system_interface) :
database_functions_(database_functions),
system_interface_(system_interface) {
}

void PrepareStatisticsAnalyzerObject::CreateStatistics(const AgentName &agent_name,
                                                       const VirtualhostName &virtualhost_name) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CreateStatistics: Function call";
  constexpr RowsCount MAX_ROWS_IN_MEMORY = 100;

  RowsCount summary_logs_count = database_functions_->GetUnusedLogsCount(agent_name, virtualhost_name);

  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CreateStatistics: Found " << summary_logs_count << " logs";

  for (RowsCount i = 0; i < summary_logs_count / MAX_ROWS_IN_MEMORY; ++i)
    CalculateStatistics(agent_name, virtualhost_name, MAX_ROWS_IN_MEMORY, i);

  auto left_offset = summary_logs_count - (summary_logs_count % MAX_ROWS_IN_MEMORY);
  auto left_logs_count = summary_logs_count % MAX_ROWS_IN_MEMORY;

  CalculateStatistics(agent_name, virtualhost_name, left_logs_count, left_offset);

  SaveAllSessions();
}

void PrepareStatisticsAnalyzerObject::CalculateStatistics(const AgentName &agent_name,
                                                          const VirtualhostName &virtualhost_name,
                                                          RowsCount count,
                                                          RowId offset) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Function call";
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Calculating statistics for " << count << " logs with offset " << offset;

  auto logs = database_functions_->GetUnusedLogs(agent_name, virtualhost_name,
                                                 count, offset);

  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Received " << logs.size() << " logs to calculate statistics";

  UniqueSessionId usi;
  for (auto log_entry : logs) {
    usi.first = log_entry.client_ip;
    usi.second = log_entry.user_agent;

    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Found new log with id: " << log_entry.id;
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Log client ip and agent name: " << log_entry.client_ip << " ; " << log_entry.agent_name;

    if (sessions_statistics_.find(usi) != sessions_statistics_.end()) {
      BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Session statistic found in memory";

      ApacheSessionEntry &session = sessions_statistics_.at(usi);

      if (IsInThisSameSession(session, log_entry)) {
        session.bandwidth_usage += log_entry.bytes;
        session.errors_count += static_cast<int> (IsErrorCode(log_entry.status_code));
        session.requests_count += 1;
        session.error_percentage = session.errors_count * 100. / session.requests_count;
        session.session_length = Distance(session.session_start.GetTime(), log_entry.time.GetTime());
      }
      else {
        BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Session statistics found in memory, but it's too old, removing";

        calculated_sessions_statistics_.push_back(session);
        sessions_statistics_.erase(usi);

        sessions_statistics_[usi] = CreateSession(log_entry);
      }
    }
    else {
      BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Session statistic not found in memory, creating new";
      sessions_statistics_[usi] = CreateSession(log_entry);
    }
  }

  database_functions_->MarkLogsAsUsed(logs);

  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Done";
}

Timestamp PrepareStatisticsAnalyzerObject::GetLastStatisticsCalculationTimestamp() {
  Timestamp last_statistics_calculation;

  auto is_stats = database_functions_->IsLastRunSet(LastRunType::STATISTICS_CALCULATION);
  if (is_stats) {
    last_statistics_calculation = database_functions_->GetLastRun(LastRunType::STATISTICS_CALCULATION);
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::GetLastRunTimestamp: Found last statistics calculation time " << last_statistics_calculation;
  }
  else {
    last_statistics_calculation.Set(0, 0, 0, 1, 1, 2016);
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::GetLastRunTimestamp: Last statistics calculation time not found - assuming " << last_statistics_calculation;
  }

  return last_statistics_calculation;
}

bool PrepareStatisticsAnalyzerObject::IsErrorCode(const int &status_code) const {
  return (status_code >= 400) && (status_code <= 511);
}

bool PrepareStatisticsAnalyzerObject::ShouldStatisticsBeCalculated(const Timestamp &last_statistics_calculation,
                                                                   const Timestamp &now) {
  return (last_statistics_calculation.GetDate() != now.GetDate()) ||
      (Distance(last_statistics_calculation.GetTime(), now.GetTime()) > SESSION_LENGTH);
}

bool PrepareStatisticsAnalyzerObject::IsInThisSameSession(const ApacheSessionEntry &session,
                                                          const ApacheLogEntry &log_entry) {
  return ( Distance(session.session_start.GetTime(), log_entry.time.GetTime()) < SESSION_LENGTH) &&
      (session.session_start.GetDate() == log_entry.time.GetDate());
}

ApacheSessionEntry PrepareStatisticsAnalyzerObject::CreateSession(const ApacheLogEntry &log_entry) {
  ApacheSessionEntry session;

  session.agent_name = log_entry.agent_name;
  session.bandwidth_usage = log_entry.bytes;
  session.client_ip = log_entry.client_ip;
  session.errors_count = static_cast<int> (IsErrorCode(log_entry.status_code));
  session.error_percentage = session.errors_count * 100;
  session.is_anomaly = false;
  session.requests_count = 1;
  session.session_length = 0;
  session.session_start = log_entry.time;
  session.useragent = log_entry.user_agent;
  session.virtualhost = log_entry.virtualhost;

  return session;
}

void PrepareStatisticsAnalyzerObject::SaveAllSessions() {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::SaveAllSessions: Function call";

  for (auto element : sessions_statistics_)
    calculated_sessions_statistics_.push_back(element.second);

  database_functions_->AddSessionStatistics(calculated_sessions_statistics_);

  sessions_statistics_.clear();
  calculated_sessions_statistics_.clear();
}

}

}

}
