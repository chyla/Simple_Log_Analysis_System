/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "get_anomaly_detection_configuration.h"

#include <boost/log/trivial.hpp>
#include <json/json.hpp>

using namespace std;
using namespace nlohmann;

namespace bash
{

namespace web
{

namespace detail
{

GetAnomalyDetectionConfigurationPtr GetAnomalyDetectionConfiguration::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return GetAnomalyDetectionConfigurationPtr(new GetAnomalyDetectionConfiguration(scripts));
}

::web::type::Command GetAnomalyDetectionConfiguration::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetUnconfiguredAgents::GetCommandName: Function call";

  return "bash.get_anomaly_detection_configurations";
}

::web::type::JsonMessage GetAnomalyDetectionConfiguration::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetUnconfiguredAgents::Execute: Function call";

  auto configurations = scripts_->GetAnomalyDetectionConfigurations();

  json j, r = json::array();
  for (const auto &c : configurations) {
    json t;
    t["id"] = c.id;
    t["agent_name_id"] = c.agent_name_id;
    t["agent_name"] = c.agent_name;
    t["begin_date"] = c.begin_date.ToString();
    t["end_date"] = c.end_date.ToString();

    r.push_back(t);
  }

  j["status"] = "ok";
  j["result"] = r;

  return j.dump();
}

GetAnomalyDetectionConfiguration::GetAnomalyDetectionConfiguration(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
