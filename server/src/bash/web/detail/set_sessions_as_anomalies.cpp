/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "set_sessions_as_anomalies.h"

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

SetSessionsAsAnomaliesPtr SetSessionsAsAnomalies::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return SetSessionsAsAnomaliesPtr(new SetSessionsAsAnomalies(scripts));
}

::web::type::Command SetSessionsAsAnomalies::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::SelectDefaultCommands::GetCommandName: Function call";

  return "bash.set_sessions_as_anomalies";
}

::web::type::JsonMessage SetSessionsAsAnomalies::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::SelectDefaultCommands::Execute: Function call";

  auto json_object = json::parse(message);
  auto args = json_object["args"];

  scripts_->UpdateDailyUserStatisticsClassification(args.at(0), args.at(1), args.at(2));

  json j;
  j["status"] = "ok";

  return j.dump();
}

SetSessionsAsAnomalies::SetSessionsAsAnomalies(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}

