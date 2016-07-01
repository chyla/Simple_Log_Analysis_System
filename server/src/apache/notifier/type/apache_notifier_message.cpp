/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "apache_notifier_message.h"
#include "src/database/detail/general_database_functions_interface.h"
#include "src/apache/database/database_functions.h"

#include <boost/log/trivial.hpp>

using namespace std;

namespace apache
{

namespace notifier
{

namespace type
{

ApacheNotifierMessagePtr ApacheNotifierMessage::Create(::apache::analyzer::type::KnnAnalyzerSummary summary,
                                                       ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                                       ::apache::database::DatabaseFunctionsPtr apache_database_functions) {
  return ApacheNotifierMessagePtr(new ApacheNotifierMessage(summary,
                                                            general_database_functions,
                                                            apache_database_functions));
}

std::string ApacheNotifierMessage::GetModuleName() {
  BOOST_LOG_TRIVIAL(debug) << "apache::notifier::ApacheNotifierMessage::GetModuleName: Function call";
  return "Apache Logs Analyzer";
}

std::string ApacheNotifierMessage::GetDetectionResults() {
  BOOST_LOG_TRIVIAL(debug) << "apache::notifier::ApacheNotifierMessage::GetDetectionResults: Function call";

  std::string results;

  for (auto stats : summary_) {
    results = "Results for agent " + general_database_functions_->GetAgentNameById(stats.agent_id) +
        " and virtualhost " + apache_database_functions_->GetVirtualhostNameById(stats.virtualhost_id) +
        "\r\n  Found " + to_string(stats.anomaly_sessions_ids.size()) + " anomalies. " +
        "\r\n  Sessions marked as anomalies: \r\n\r\n";

    for (auto i : stats.anomaly_sessions_ids) {
      const auto s = apache_database_functions_->GetOneSessionStatistic(i);

      results += "    ...........................................................................\r\n";
      results += "    ID:                      " + to_string(i) + "\r\n";
      results += "    IP:                      " + s.client_ip + "\r\n";
      results += "    Session start:           " + s.session_start.ToString() + "\r\n";
      results += "    Session length (s):      " + to_string(s.session_length) + "\r\n";
      results += "    Bandwidth usage (bytes): " + to_string(s.bandwidth_usage) + "\r\n";
      results += "    Requests count:          " + to_string(s.requests_count) + "\r\n";
      results += "    Error requests (%):      " + to_string(s.error_percentage) + "\r\n";
      results += "    User-Agent:              " + s.useragent + "\r\n";
      results += "    ...........................................................................\r\n";

      results += "\r\n";
    }

    results += "--------------------------------------------------------------------------------\r\n\r\n";
  }

  return results;
}

ApacheNotifierMessage::ApacheNotifierMessage(::apache::analyzer::type::KnnAnalyzerSummary summary,
                                             ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                             ::apache::database::DatabaseFunctionsPtr apache_database_functions) :
summary_(summary),
general_database_functions_(general_database_functions),
apache_database_functions_(apache_database_functions) {
}

}

}

}
