/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "knn_analyzer_object.h"

#include <boost/log/trivial.hpp>
#include <cmath>
#include <limits>

#include "system.h"

using namespace ::apache::database::detail;
using namespace ::apache::type;
using namespace ::database::type;
using namespace ::type;

namespace apache
{

namespace analyzer
{

namespace detail
{

KnnAnalyzerObjectPtr KnnAnalyzerObject::Create(::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                               ::apache::database::DatabaseFunctionsPtr apache_database_functions) {
  auto system = System::Create();

  return Create(system, general_database_functions, apache_database_functions);
}

KnnAnalyzerObjectPtr KnnAnalyzerObject::Create(detail::SystemInterfacePtr system_interface,
                                               ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                               ::apache::database::DatabaseFunctionsPtr apache_database_functions) {
  return KnnAnalyzerObjectPtr(new KnnAnalyzerObject(system_interface, general_database_functions, apache_database_functions));
}

void KnnAnalyzerObject::Analyze() {
  auto now = GetCurrentTimestamp();
  auto last_analyze = GetLastAnalyzeTimestamp(now);

  for (auto agent_name : general_database_functions_->GetAgentNames()) {
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::Analyze: Preparing agent: " << agent_name;

    for (auto virtualhost_name : apache_database_functions_->GetVirtualhostNames(agent_name)) {
      BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::Analyze: Preparing virtualhost: " << virtualhost_name;

      // sprawdzic istnienie konfiguracji
      AnalyzeVirtualhost(agent_name, virtualhost_name, last_analyze, now);
    }
  }

  apache_database_functions_->SetLastRun(LastRunType::ANALYZING, now);
}

void KnnAnalyzerObject::AnalyzeVirtualhost(const ::database::type::AgentName &agent_name,
                                           const ::database::type::VirtualhostName &virtualhost_name,
                                           const ::type::Timestamp &last_analyze_timestamp,
                                           const ::type::Timestamp &now) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeVirtualhost: Function call";
  constexpr RowsCount MAX_ROWS_IN_MEMORY = 100;

  auto agent_id = general_database_functions_->GetAgentNameId(agent_name);
  auto virtualhost_id = apache_database_functions_->GetVirtualhostNameId(virtualhost_name);

  auto from = last_analyze_timestamp;
  auto count = apache_database_functions_->GetLearningSessionsCount(agent_id, virtualhost_id);
  for (auto session_id : apache_database_functions_->GetLearningSessionsIds(agent_id, virtualhost_id, count, 0))
  {
    auto session = apache_database_functions_->GetOneSessionStatistic(session_id);
    if (from < session.session_start)
      from = session.session_start;
  }
  from = ::Timestamp::Create(from.GetTime() + 1, from.GetDate());
  auto to = now;

  auto sessions_count = apache_database_functions_->GetSessionStatisticsCount(agent_name, virtualhost_name, from, to);

  for (RowsCount i = 0; i < sessions_count / MAX_ROWS_IN_MEMORY; ++i)
    AnalyzeSessions(agent_name, virtualhost_name, from, to, MAX_ROWS_IN_MEMORY, i);

  auto left_offset = sessions_count - (sessions_count % MAX_ROWS_IN_MEMORY);
  auto left_logs_count = sessions_count % MAX_ROWS_IN_MEMORY;

  AnalyzeSessions(agent_name, virtualhost_name, from, to, left_logs_count, left_offset);
}

void KnnAnalyzerObject::AnalyzeSessions(const ::database::type::AgentName &agent_name,
                                        const ::database::type::VirtualhostName &virtualhost_name,
                                        const ::type::Timestamp &from,
                                        const ::type::Timestamp &to,
                                        unsigned limit,
                                        ::database::type::RowsCount offset) {
  constexpr RowsCount MAX_ROWS_IN_MEMORY = 100;

  auto sessions_part = apache_database_functions_->GetSessionStatistics(agent_name, virtualhost_name,
                                                                        from, to,
                                                                        limit, offset);

  auto agent_id = general_database_functions_->GetAgentNameId(agent_name);
  auto virtualhost_id = apache_database_functions_->GetVirtualhostNameId(virtualhost_name);

  auto learning_set_count = apache_database_functions_->GetLearningSessionsCount(agent_id, virtualhost_id);

  for (auto session : sessions_part) {
    for (auto dsi : distance_table_) {
      dsi.first = 0;
      dsi.second = -1;
    }

    for (RowsCount i = 0; i < learning_set_count / MAX_ROWS_IN_MEMORY; ++i)
      AnalyzeSessionsWithLearningSet(agent_id, virtualhost_id, MAX_ROWS_IN_MEMORY, i, session);

    auto left_offset = learning_set_count - (learning_set_count % MAX_ROWS_IN_MEMORY);
    auto left_logs_count = learning_set_count % MAX_ROWS_IN_MEMORY;

    AnalyzeSessionsWithLearningSet(agent_id, virtualhost_id, left_logs_count, left_offset, session);

    session.is_anomaly =
        (distance_table_.at(0).first +
        distance_table_.at(1).first +
        distance_table_.at(2).first) > 1;
  }
}

void KnnAnalyzerObject::AnalyzeSessionsWithLearningSet(const ::database::type::RowId &agent_name_id,
                                                       const ::database::type::RowId &virtualhost_name_id,
                                                       unsigned limit,
                                                       ::database::type::RowsCount offset,
                                                       const ::apache::type::ApacheSessionEntry &session_to_analyze) {
  auto learning_set_ids = apache_database_functions_->GetLearningSessionsIds(agent_name_id, virtualhost_name_id, limit, offset);

  for (auto session_id : learning_set_ids) {
    double d = Distance(session_to_analyze,
                        apache_database_functions_->GetOneSessionStatistic(session_id));

    for (auto dsi : distance_table_) {
      if (dsi.first < d || dsi.second == -1) {
        dsi.first = d;
        dsi.second = session_id;
        break;
      }
    }
  }
}

double KnnAnalyzerObject::Distance(const ::apache::type::ApacheSessionEntry &a,
                                   const ::apache::type::ApacheSessionEntry &b) const {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::Distance: Function call";

  double d = sqrt(pow(a.session_length - b.session_length, 2) +
                  pow(a.bandwidth_usage - b.bandwidth_usage, 2) +
                  pow(a.requests_count - b.requests_count, 2) +
                  pow(a.error_percentage - b.error_percentage, 2)
                  );

  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::Distance: Distance between session id=" << a.id << "; id2=" << b.id << " is " << d;

  return d;
}

KnnAnalyzerObject::KnnAnalyzerObject(detail::SystemInterfacePtr system_interface,
                                     ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                     ::apache::database::DatabaseFunctionsPtr apache_database_functions) :
system_interface_(system_interface),
general_database_functions_(general_database_functions),
apache_database_functions_(apache_database_functions) {
}

Timestamp KnnAnalyzerObject::GetCurrentTimestamp() const {
  time_t t = system_interface_->Time(nullptr);
  struct tm *now = system_interface_->LocalTime(&t);

  return Timestamp::Create(Time::Create(now->tm_hour, now->tm_min, now->tm_sec),
                           Date::Create(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900));
}

::type::Timestamp KnnAnalyzerObject::GetLastAnalyzeTimestamp(const ::type::Timestamp &now) const {
  Timestamp last_analyze;

  auto is_stats = apache_database_functions_->IsLastRunSet(LastRunType::ANALYZING);
  if (is_stats) {
    last_analyze = apache_database_functions_->GetLastRun(LastRunType::ANALYZING);
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::GetLastAnalyzeTimestamp: Found last analyze time " << last_analyze;
  }
  else {
    last_analyze.Set(0, 0, 0, 1, 1, 2016);
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::GetLastAnalyzeTimestamp: Last analyze time not found - assuming " << last_analyze;
  }

  return last_analyze;
}

}

}

}
