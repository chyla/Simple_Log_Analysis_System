/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "knn_analyzer_object.h"

#include <boost/log/trivial.hpp>
#include <cmath>
#include <limits>
#include <slas/util/run_partially.h>

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

void KnnAnalyzerObject::Analyze() {
  for (auto agent_name : general_database_functions_->GetAgentNames()) {
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::Analyze: Preparing agent: " << agent_name;

    for (auto virtualhost_name : apache_database_functions_->GetVirtualhostNames(agent_name)) {
      BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::Analyze: Preparing virtualhost: " << virtualhost_name;

      auto agent_id = general_database_functions_->GetAgentNameId(agent_name);
      auto virtualhost_id = apache_database_functions_->GetVirtualhostNameId(virtualhost_name);
      BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::Analyze: Agent ID: " << agent_id;
      BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::Analyze: Virtualhost ID " << virtualhost_id;

      AnalyzeVirtualhost(agent_id, virtualhost_id);
    }
  }
}

bool KnnAnalyzerObject::IsAnomalyDetected() const {
  return is_anomaly_detected_;
}

::apache::analyzer::type::KnnAnalyzerSummary KnnAnalyzerObject::GetAnalyzeSummary() const {
  return analyze_summary_;
}

void KnnAnalyzerObject::AnalyzeVirtualhost(const ::database::type::RowId &agent_name_id,
                                           const ::database::type::RowId &virtualhost_name_id) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeVirtualhost: Function call";
  constexpr RowsCount MAX_ROWS_IN_MEMORY = 100;
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Max rows in memory: " << MAX_ROWS_IN_MEMORY;

  auto sessions_count = apache_database_functions_->GetNotClassifiedSessionsStatisticsCount(agent_name_id, virtualhost_name_id);
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeVirtualhost: Found " << sessions_count << " sessions to analyze";

  analyze_summary_.push_back(type::KnnVirtualhostAnalyzeStatistics());

  util::RunPartially(MAX_ROWS_IN_MEMORY, sessions_count, [&](long long part_count, long long offset) {
    AnalyzeSessions(agent_name_id, virtualhost_name_id, part_count, offset);
  });
}

void KnnAnalyzerObject::AnalyzeSessions(const ::database::type::RowId &agent_name_id,
                                        const ::database::type::RowId &virtualhost_name_id,
                                        unsigned limit,
                                        ::database::type::RowsCount offset) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Function call";
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Analyzing sessions: agent_name_id=" << agent_name_id << "; virtualhost_name_id=" << virtualhost_name_id << " (limit=" << limit << "; offset=" << offset << ")";
  constexpr RowsCount MAX_ROWS_IN_MEMORY = 100;

  auto sessions_part = apache_database_functions_->GetNotClassifiedSessionStatistics(agent_name_id, virtualhost_name_id, limit, offset);
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Received " << sessions_part.size() << " sessions statictics";

  type::KnnVirtualhostAnalyzeStatistics &stats = analyze_summary_.at(analyze_summary_.size() - 1);
  stats.agent_id = agent_name_id;
  stats.virtualhost_id = virtualhost_name_id;

  auto learning_set_count = apache_database_functions_->GetLearningSessionsCount(agent_name_id, virtualhost_name_id);
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessions: Found " << learning_set_count << " sessions in learning set";

  for (auto session : sessions_part) {
    neighbours_table_.SetSession(session);

    util::RunPartially(MAX_ROWS_IN_MEMORY, learning_set_count, [&](long long part_count, long long offset) {
      AnalyzeSessionsWithLearningSet(agent_name_id, virtualhost_name_id, part_count, 0, session);
    });

    auto classification = GetSessionClassification();

    if (classification == ::database::type::Classification::ANOMALY) {
      is_anomaly_detected_ = true;
      stats.anomaly_sessions_ids.push_back(session.id);
    }

    session.classification = classification;
    apache_database_functions_->UpdateSessionStatisticClassification(session.id, session.classification);

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

    if (learning_set_session.classification != ::database::type::Classification::UNKNOWN)
      neighbours_table_.Add(learning_set_session);
    else
      BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessionsWithLearningSet: Unknown session classification (id=" << learning_set_session.id << "(";
  }
}

::database::type::Classification KnnAnalyzerObject::GetSessionClassification() {
  long long i = 0;
  const auto &neighbours = neighbours_table_.Get();

  for (const auto &n : neighbours)
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::IsSessionAnomaly: Final nearest neighbour id=" << n.session_id;

  for (const auto &n : neighbours)
    i = i - 1 + 2 * static_cast<int> (n.classification == ::database::type::Classification::ANOMALY);

  ::database::type::Classification c = ::database::type::Classification::UNKNOWN;
  if (neighbours.size() > 0) {
    if (i > 0)
      c = ::database::type::Classification::ANOMALY;
    else
      c = ::database::type::Classification::NORMAL;
  }

  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::IsSessionAnomaly: Classification: " << static_cast<int> (c);

  return c;
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
