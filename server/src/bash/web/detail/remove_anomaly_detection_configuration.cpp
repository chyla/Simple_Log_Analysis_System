/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "remove_anomaly_detection_configuration.h"

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

RemoveAnomalyDetectionConfigurationPtr RemoveAnomalyDetectionConfiguration::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return RemoveAnomalyDetectionConfigurationPtr(new RemoveAnomalyDetectionConfiguration(scripts));
}

::web::type::Command RemoveAnomalyDetectionConfiguration::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetUnconfiguredAgents::GetCommandName: Function call";

  return "bash.remove_anomaly_detection_configurations";
}

::web::type::JsonMessage RemoveAnomalyDetectionConfiguration::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetUnconfiguredAgents::Execute: Function call";

  auto json_object = json::parse(message);
  auto args = json_object["args"];

  scripts_->RemoveAnomalyDetectionConfiguration(args.at(0));

  json j;
  j["status"] = "ok";

  return j.dump();
}

RemoveAnomalyDetectionConfiguration::RemoveAnomalyDetectionConfiguration(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
