/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "save_anomaly_detection_configuration.h"

#include <boost/log/trivial.hpp>
#include <json/json.hpp>
#include <slas/type/date.h>

using namespace std;
using namespace nlohmann;

namespace bash
{

namespace web
{

namespace detail
{

SaveAnomalyDetectionConfigurationPtr SaveAnomalyDetectionConfiguration::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return SaveAnomalyDetectionConfigurationPtr(new SaveAnomalyDetectionConfiguration(scripts));
}

::web::type::Command SaveAnomalyDetectionConfiguration::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetUnconfiguredAgents::GetCommandName: Function call";

  return "bash.save_anomaly_detection_configuration";
}

::web::type::JsonMessage SaveAnomalyDetectionConfiguration::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetUnconfiguredAgents::Execute: Function call";

  auto json_object = json::parse(message);
  auto args = json_object["args"];

  scripts_->SaveConfiguration(args.at(0),
                              ::type::Date::Create(args.at(1)),
                              ::type::Date::Create(args.at(2)));

  json j;
  j["status"] = "ok";

  return j.dump();
}

SaveAnomalyDetectionConfiguration::SaveAnomalyDetectionConfiguration(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
