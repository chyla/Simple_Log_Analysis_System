/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "get_unconfigured_agents.h"

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

GetUnconfiguredAgentsPtr GetUnconfiguredAgents::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return GetUnconfiguredAgentsPtr(new GetUnconfiguredAgents(scripts));
}

::web::type::Command GetUnconfiguredAgents::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetUnconfiguredAgents::GetCommandName: Function call";

  return "bash.get_unconfigured_agents";
}

::web::type::JsonMessage GetUnconfiguredAgents::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetUnconfiguredAgents::Execute: Function call";

  auto agents = scripts_->GetUnconfigurentAgents();

  json j, r = json::array();
  for (const auto &a : agents) {
    json t;
    t["id"] = a.id;
    t["name"] = a.name;

    r.push_back(t);
  }

  j["status"] = "ok";
  j["result"] = r;

  return j.dump();
}

GetUnconfiguredAgents::GetUnconfiguredAgents(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
