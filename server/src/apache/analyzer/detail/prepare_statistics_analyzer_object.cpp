/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "prepare_statistics_analyzer_object.h"

#include <boost/log/trivial.hpp>
#include <patlms/util/distance.h>

#include "system.h"
#include "session_length.h"

using namespace ::database::type;
using namespace ::type;
using namespace ::util;

namespace apache
{

namespace analyzer
{

namespace detail
{

PrepareStatisticsAnalyzerObjectPtr PrepareStatisticsAnalyzerObject::Create(::apache::database::detail::DatabaseFunctionsInterfacePtr database_functions) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::Create: Function call";
  auto system = System::Create();

  return PrepareStatisticsAnalyzerObjectPtr(new PrepareStatisticsAnalyzerObject(database_functions, system));
}

PrepareStatisticsAnalyzerObjectPtr PrepareStatisticsAnalyzerObject::Create(::apache::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                                                           SystemInterfacePtr system_interface) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::Create: Function call";

  return PrepareStatisticsAnalyzerObjectPtr(new PrepareStatisticsAnalyzerObject(database_functions, system_interface));
}

void PrepareStatisticsAnalyzerObject::Prepare() {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::Prepare: Function call";

  const auto last_statistics_calculation = GetLastStatisticsCalculationTimestamp();
  const auto now = GetCurrentTimestamp();

  if ((last_statistics_calculation.GetDate() != now.GetDate()) ||
      (Distance(last_statistics_calculation.GetTime(), now.GetTime()) > SESSION_LENGTH)) {
    for (auto agent_name : database_functions_->GetAgentNames()) {
      BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::Prepare: Preparing agent: " << agent_name;

      for (auto virtualhost_name : database_functions_->GetVirtualhostNames(agent_name)) {
        BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::Prepare: Preparing virtualhost: " << virtualhost_name;

        CreateStatistics(agent_name, virtualhost_name, now);
      }
    }

    database_functions_->SetLastRun(::apache::type::LastRunType::STATISTICS_CALCULATION, now);
  }
  else {
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::Prepare: Not running, last statistics prepared: " << GetLastStatisticsCalculationTimestamp();
  }
}

PrepareStatisticsAnalyzerObject::PrepareStatisticsAnalyzerObject(::apache::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                                                 SystemInterfacePtr system_interface) :
database_functions_(database_functions),
system_interface_(system_interface) {
}

void PrepareStatisticsAnalyzerObject::CreateStatistics(const AgentName &agent_name,
                                                       const VirtualhostName &virtualhost_name,
                                                       const Timestamp &now) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CreateStatistics: Function call";

  const auto last_statistics_calculation = GetLastStatisticsCalculationTimestamp();

  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CreateStatistics: Last run " << last_statistics_calculation;
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CreateStatistics: Today date " << now;

  CreateStatisticsForPastDays(agent_name, virtualhost_name, last_statistics_calculation, now);
  CreateStatisticsForToday(agent_name, virtualhost_name, last_statistics_calculation, now);
}

void PrepareStatisticsAnalyzerObject::CreateStatisticsForPastDays(const AgentName &agent_name,
                                                                  const VirtualhostName &virtualhost_name,
                                                                  const Timestamp &last_statistics_calculation,
                                                                  const Timestamp &now) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CreateStatisticsForPastDays: Function call";

  const auto date_from = last_statistics_calculation.GetDate();
  const auto date_to = now.GetDate();

  for (auto date = date_from; date < date_to; date = date.GetTomorrowDate()) {
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CreateStatisticsForPastDays: Creating session statistics for " << date;

    if (database_functions_->AreStatisticsCreatedFor(date) == false) {
      BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CreateStatisticsForPastDays: Statistics not created for " << date;

      CalculateStatisticsPartially(agent_name,
                                   virtualhost_name,
                                   Timestamp::Create(::type::Time::Create(0, 0, 0), date),
                                   Timestamp::Create(::type::Time::Create(23, 59, 59), date));

      database_functions_->MarkStatisticsAsCreatedFor(date);
    }
    else {
      BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CreateStatisticsForPastDays: Statistics already created for " << date << ", nothing to do";
    }
  }
}

void PrepareStatisticsAnalyzerObject::CreateStatisticsForToday(const AgentName &agent_name,
                                                               const VirtualhostName &virtualhost_name,
                                                               const Timestamp &last_statistics_calculation,
                                                               const Timestamp &now) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CreateStatisticsForToday: Function call";

  Time from_time;
  if (last_statistics_calculation.GetDate() == now.GetDate())
    from_time = last_statistics_calculation.GetTime();
  else
    from_time.Set(0, 0, 0);

  Timestamp from = Timestamp::Create(from_time, now.GetDate());
  const Timestamp &to = now;
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CreateStatisticsForToday: Creating session statistics for today (" << now << ") partially, from " << from << " to " << now;

  CalculateStatisticsPartially(agent_name, virtualhost_name, from, to);
}

void PrepareStatisticsAnalyzerObject::CalculateStatisticsPartially(const AgentName &agent_name,
                                                                   const VirtualhostName &virtualhost_name,
                                                                   const Timestamp &from,
                                                                   const Timestamp &to) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatisticsPartially: Function call";
  constexpr RowsCount MAX_ROWS_IN_MEMORY = 100;

  RowsCount summary_logs_count = database_functions_->GetLogsCount(agent_name, virtualhost_name, from, to);

  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatisticsPartially: Found " << summary_logs_count << " logs";

  for (RowsCount i = 0; i < summary_logs_count / MAX_ROWS_IN_MEMORY; ++i)
    CalculateStatistics(agent_name, virtualhost_name, from, to, MAX_ROWS_IN_MEMORY, i);

  auto left_offset = summary_logs_count - (summary_logs_count % MAX_ROWS_IN_MEMORY);
  auto left_logs_count = summary_logs_count % MAX_ROWS_IN_MEMORY;

  CalculateStatistics(agent_name, virtualhost_name, from, to, left_logs_count, left_offset);

  SaveAllSessions();
}

void PrepareStatisticsAnalyzerObject::CalculateStatistics(const AgentName &agent_name,
                                                          const VirtualhostName &virtualhost_name,
                                                          const Timestamp &from,
                                                          const Timestamp &to,
                                                          RowsCount count,
                                                          RowId offset) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Function call";
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Calculating statistics for " << count << " logs with offset " << offset << " from " << from << " to " << to;

  auto logs = database_functions_->GetLogs(agent_name, virtualhost_name,
                                           from, to,
                                           count, offset);

  UniqueSessionId usi;
  for (auto log_entry : logs) {
    usi.first = log_entry.client_ip;
    usi.second = log_entry.agent_name;

    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Found new log with id: " << log_entry.id;
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Log client ip and agent name: " << log_entry.client_ip << " ; " << log_entry.agent_name;

    if (sessions_statistics_.find(usi) != sessions_statistics_.end()) {
      BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Session statistics found in memory";

      ::apache::type::ApacheSessionEntry &session = sessions_statistics_.at(usi);

      if (IsInThisSameSession(session, log_entry)) {
        session.bandwidth_usage += log_entry.bytes;
        session.errors_count += static_cast<int> (IsErrorCode(log_entry.status_code));
        session.requests_count += 1;
        session.error_percentage = session.errors_count * 100. / session.requests_count;
        session.session_length = Distance(session.session_start.GetTime(), log_entry.time.GetTime());
      }
      else {
        BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Session statistics found in memory, but removing";

        calculated_sessions_statistics_.push_back(session);
        sessions_statistics_.erase(usi);

        sessions_statistics_[usi] = CreateSession(log_entry);
      }
    }
    else {
      BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Session statistics not found in memory";
      sessions_statistics_[usi] = CreateSession(log_entry);
    }
  }

  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::CalculateStatistics: Done";
}

::type::Timestamp PrepareStatisticsAnalyzerObject::GetLastStatisticsCalculationTimestamp() {
  Timestamp last_statistics_calculation;

  auto is_stats = database_functions_->IsLastRunSet(::apache::type::LastRunType::STATISTICS_CALCULATION);
  if (is_stats) {
    last_statistics_calculation = database_functions_->GetLastRun(::apache::type::LastRunType::STATISTICS_CALCULATION);
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::GetLastRunTimestamp: Found last statistics calculation time " << last_statistics_calculation;
  }
  else {
    last_statistics_calculation.Set(0, 0, 0, 1, 1, 2016);
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::PrepareStatisticsAnalyzerObject::GetLastRunTimestamp: Last statistics calculation time not found - assuming " << last_statistics_calculation;
  }

  return last_statistics_calculation;
}

::type::Timestamp PrepareStatisticsAnalyzerObject::GetCurrentTimestamp() const {
  time_t t = system_interface_->Time(nullptr);
  struct tm *now = system_interface_->LocalTime(&t);

  return Timestamp::Create(::type::Time::Create(now->tm_hour, now->tm_min, now->tm_sec),
                           Date::Create(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900));
}

bool PrepareStatisticsAnalyzerObject::IsErrorCode(const int &status_code) const {
  return (status_code >= 400) && (status_code <= 511);
}

bool PrepareStatisticsAnalyzerObject::IsInThisSameSession(const ::apache::type::ApacheSessionEntry &session,
                                                          const ApacheLogEntry &log_entry) {
  return ( Distance(session.session_start.GetTime(), log_entry.time.GetTime()) < SESSION_LENGTH) &&
      (session.session_start.GetDate() == log_entry.time.GetDate());
}

::apache::type::ApacheSessionEntry PrepareStatisticsAnalyzerObject::CreateSession(const ApacheLogEntry &log_entry) {
  ::apache::type::ApacheSessionEntry session;

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
