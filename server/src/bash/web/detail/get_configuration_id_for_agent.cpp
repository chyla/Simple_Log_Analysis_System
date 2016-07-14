/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "get_configuration_id_for_agent.h"

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

GetConfigurationIdForAgentPtr GetConfigurationIdForAgent::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return GetConfigurationIdForAgentPtr(new GetConfigurationIdForAgent(scripts));
}

::web::type::Command GetConfigurationIdForAgent::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetConfigurationIdForAgent::GetCommandName: Function call";

  return "bash.get_configuration_id_for_agent";
}

::web::type::JsonMessage GetConfigurationIdForAgent::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetConfigurationIdForAgent::Execute: Function call";

  auto json_object = json::parse(message);
  auto args = json_object["args"];

  auto id = scripts_->GetConfigurationIdForAgent(args.at(0));

  json j;
  j["status"] = "ok";
  j["result"] = id;

  return j.dump();
}

GetConfigurationIdForAgent::GetConfigurationIdForAgent(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
