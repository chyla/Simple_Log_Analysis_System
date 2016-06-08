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
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Max rows in memory: " << MAX_ROWS_IN_MEMORY;
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Last analyze timestamp: " << last_analyze_timestamp;
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Current timestamp: " << now;

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
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeVirtualhost: Found " << sessions_count << " sessions to analyze";

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
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Function call";
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Analyzing sessions: " << agent_name << "/" << virtualhost_name << " from " << from << " to " << to << " (limit=" << limit << "; offset=" << offset << ")";
  constexpr RowsCount MAX_ROWS_IN_MEMORY = 100;

  auto sessions_part = apache_database_functions_->GetSessionStatistics(agent_name, virtualhost_name,
                                                                        from, to,
                                                                        limit, offset);
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Received " << sessions_part.size() << " sessions statictics";

  auto agent_id = general_database_functions_->GetAgentNameId(agent_name);
  auto virtualhost_id = apache_database_functions_->GetVirtualhostNameId(virtualhost_name);
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Agent ID: " << agent_id;
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Virtualhost ID " << virtualhost_id;

  auto learning_set_count = apache_database_functions_->GetLearningSessionsCount(agent_id, virtualhost_id);
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Found " << learning_set_count << " sessions in learning set";

  for (auto session : sessions_part) {
    for (auto i = 0; i < distance_table_.size(); ++i) {
      distance_table_[i].first = 0;
      distance_table_[i].second = -1;
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
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Is session with id " << session.id << " anomaly? - " << session.is_anomaly;
  }
}

void KnnAnalyzerObject::AnalyzeSessionsWithLearningSet(const ::database::type::RowId &agent_name_id,
                                                       const ::database::type::RowId &virtualhost_name_id,
                                                       unsigned limit,
                                                       ::database::type::RowsCount offset,
                                                       const ::apache::type::ApacheSessionEntry &session_to_analyze) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessionsWithLearningSet: Function call";
  auto learning_set_ids = apache_database_functions_->GetLearningSessionsIds(agent_name_id, virtualhost_name_id, limit, offset);

  for (auto session_id : learning_set_ids) {
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessionsWithLearningSet: Comparing session to analyze with id=" << session_to_analyze.id << " to session from learning set " << session_id;

    double d = Distance(session_to_analyze,
                        apache_database_functions_->GetOneSessionStatistic(session_id));

    for (auto dsi : distance_table_) {
      if (dsi.first < d || dsi.second == -1) {
        BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessionsWithLearningSet: Updating distance table";

        dsi.first = d;
        dsi.second = session_id;
        break;
      }
    }

    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessionsWithLearningSet: New distance table: "
        << "(" << distance_table_.at(0).first << "; " << distance_table_.at(0).second << "), "
        << "(" << distance_table_.at(1).first << "; " << distance_table_.at(1).second << "), "
        << "(" << distance_table_.at(2).first << "; " << distance_table_.at(2).second << "), ";
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
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::GetCurrentTimestamp: Function call";
  time_t t = system_interface_->Time(nullptr);
  struct tm *now = system_interface_->LocalTime(&t);

  return Timestamp::Create(Time::Create(now->tm_hour, now->tm_min, now->tm_sec),
                           Date::Create(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900));
}

::type::Timestamp KnnAnalyzerObject::GetLastAnalyzeTimestamp(const ::type::Timestamp &now) const {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::GetLastAnalyzeTimestamp: Function call";
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