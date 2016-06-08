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

void KnnAnalyzerObject::Analyze(const ::type::Timestamp &now) {
  auto is_stats = apache_database_functions_->IsLastRunSet(LastRunType::ANALYZING);

  if (is_stats) {
    auto last_analyze = apache_database_functions_->GetLastRun(LastRunType::ANALYZING);
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
    for (unsigned i = 0; i < distance_table_.size(); ++i)
      distance_table_[i].ClearValues();

    for (RowsCount i = 0; i < learning_set_count / MAX_ROWS_IN_MEMORY; ++i)
      AnalyzeSessionsWithLearningSet(agent_id, virtualhost_id, MAX_ROWS_IN_MEMORY, i, session);

    auto left_offset = learning_set_count - (learning_set_count % MAX_ROWS_IN_MEMORY);
    auto left_logs_count = learning_set_count % MAX_ROWS_IN_MEMORY;

    AnalyzeSessionsWithLearningSet(agent_id, virtualhost_id, left_logs_count, left_offset, session);

    session.is_anomaly =
        (distance_table_.at(0).is_session_anomaly +
        distance_table_.at(1).is_session_anomaly +
        distance_table_.at(2).is_session_anomaly) > 1;
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

    auto learning_set_session = apache_database_functions_->GetOneSessionStatistic(session_id);

    double d = Distance(session_to_analyze, learning_set_session);

    if (IsSessionInDistanceTable(learning_set_session.id) == false) {
      for (unsigned i = 0; i < distance_table_.size(); ++i) {
        if ((d < distance_table_[i].distance) || (distance_table_[i].distance < 0)) {
          BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessionsWithLearningSet: Updating distance table";

          distance_table_[i].distance = d;
          distance_table_[i].is_session_anomaly = learning_set_session.is_anomaly;
          distance_table_[i].session_id = learning_set_session.id;
          break;
        }
      }
    }

    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::AnalyzeSessionsWithLearningSet: New distance table: "
        << "(" << distance_table_[0].distance << "; " << distance_table_[0].is_session_anomaly << "; " << distance_table_[0].session_id << "), "
        << "(" << distance_table_[1].distance << "; " << distance_table_[1].is_session_anomaly << "; " << distance_table_[1].session_id << "), "
        << "(" << distance_table_[2].distance << "; " << distance_table_[2].is_session_anomaly << "; " << distance_table_[2].session_id << "), ";
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

bool KnnAnalyzerObject::IsSessionInDistanceTable(const ::database::type::RowId &id) {
  for (auto it = distance_table_.begin(); it != distance_table_.end(); ++it)
    if (it->session_id == id)
      return true;

  return false;
}

}

}

}
