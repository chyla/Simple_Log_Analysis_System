/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "knn_analyzer_object.h"

#include <boost/log/trivial.hpp>
#include <cmath>
#include <limits>
#include <patlms/util/run_partially.h>

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
  return KnnAnalyzerObjectPtr(new KnnAnalyzerObject(general_database_functions, apache_database_functions));
}

void KnnAnalyzerObject::Analyze(const ::type::Timestamp &now) {
  auto is_stats = apache_database_functions_->IsLastRunSet();

  if (is_stats) {
    auto last_analyze = apache_database_functions_->GetLastRun();
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::Analyze: Found last run time " << last_analyze;

    for (auto agent_name : general_database_functions_->GetAgentNames()) {
      BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::Analyze: Preparing agent: " << agent_name;

      for (auto virtualhost_name : apache_database_functions_->GetVirtualhostNames(agent_name)) {
        BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::Analyze: Preparing virtualhost: " << virtualhost_name;

        // sprawdzic istnienie konfiguracji
        AnalyzeVirtualhost(agent_name, virtualhost_name, last_analyze, now);
      }
    }
  }
}

bool KnnAnalyzerObject::IsAnomalyDetected() const {
  return is_anomaly_detected_;
}

::apache::analyzer::type::KnnAnalyzerSummary KnnAnalyzerObject::GetAnalyzeSummary() const {
  return analyze_summary_;
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

  auto from = last_analyze_timestamp;
  auto to = now;

  auto sessions_count = apache_database_functions_->GetSessionStatisticsCount(agent_name, virtualhost_name, from, to);
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeVirtualhost: Found " << sessions_count << " sessions to analyze";

  analyze_summary_.push_back(type::KnnVirtualhostAnalyzeStatistics());

  util::RunPartially(MAX_ROWS_IN_MEMORY, sessions_count, [&](long long part_count, long long offset) {
    AnalyzeSessions(agent_name, virtualhost_name, from, to, part_count, offset);
  });
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

  type::KnnVirtualhostAnalyzeStatistics &stats = analyze_summary_.at(analyze_summary_.size() - 1);
  stats.agent_id = agent_id;
  stats.virtualhost_id = virtualhost_id;

  auto learning_set_count = apache_database_functions_->GetLearningSessionsCount(agent_id, virtualhost_id);
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Found " << learning_set_count << " sessions in learning set";

  for (auto session : sessions_part) {
    neighbours_table_.SetSession(session);

    util::RunPartially(MAX_ROWS_IN_MEMORY, learning_set_count, [&](long long part_count, long long offset) {
      AnalyzeSessionsWithLearningSet(agent_id, virtualhost_id, part_count, offset, session);
    });

    if (IsSessionAnomaly()) {
      is_anomaly_detected_ = true;
      stats.anomaly_sessions_ids.push_back(session.id);

      session.classification = ::database::type::Classification::ANOMALY;
      apache_database_functions_->MarkSessionStatisticAsAnomaly(session.id);
    }

    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Is session with id " << session.id << " anomaly? - " << (session.classification == ::database::type::Classification::ANOMALY);
  }
}

void KnnAnalyzerObject::AnalyzeSessionsWithLearningSet(const ::database::type::RowId &agent_name_id,
                                                       const ::database::type::RowId &virtualhost_name_id,
                                                       unsigned limit,
                                                       ::database::type::RowsCount offset,
                                                       const ::apache::type::ApacheSessionEntry &session_to_analyze) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessionsWithLearningSet: Function call";
  auto learning_set_ids = apache_database_functions_->GetLearningSessionsIds(agent_name_id, virtualhost_name_id, limit, offset);

  for (const auto &session_id : learning_set_ids) {
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessionsWithLearningSet: Comparing session to analyze with id=" << session_to_analyze.id << " to session from learning set " << session_id;

    const auto &learning_set_session = apache_database_functions_->GetOneSessionStatistic(session_id);

    neighbours_table_.Add(learning_set_session);
  }
}

bool KnnAnalyzerObject::IsSessionAnomaly() {
  long long i = 0;
  const auto &neighbours = neighbours_table_.Get();

  for (const auto &n : neighbours)
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::IsSessionAnomaly: Final nearest neighbour id=" << n.session_id;

  for (const auto &n : neighbours)
    i = i - 1 + 2 * static_cast<int> (n.is_session_anomaly);

  bool anomaly = i > 0;

  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::IsSessionAnomaly: Anomaly: " << anomaly;

  return anomaly;
}

KnnAnalyzerObject::KnnAnalyzerObject(::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                     ::apache::database::DatabaseFunctionsPtr apache_database_functions) :
general_database_functions_(general_database_functions),
apache_database_functions_(apache_database_functions),
is_anomaly_detected_(false) {
}

}

}

}
