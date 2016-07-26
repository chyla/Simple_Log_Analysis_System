/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "get_agents_with_existing_daily_user_statistics.h"

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

GetAgentsWithExistingDailyUserStatisticsPtr GetAgentsWithExistingDailyUserStatistics::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return GetAgentsWithExistingDailyUserStatisticsPtr(new GetAgentsWithExistingDailyUserStatistics(scripts));
}

::web::type::Command GetAgentsWithExistingDailyUserStatistics::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetAgentsWithExistingDailyUserStatistics::GetCommandName: Function call";

  return "bash.get_agents_with_existing_sessions";
}

::web::type::JsonMessage GetAgentsWithExistingDailyUserStatistics::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetAgentsWithExistingDailyUserStatistics::Execute: Function call";

  auto agents = scripts_->GetAgentsWithExistingDailyUserStatistics();

  json j, r = json::array();
  for (const auto &a : agents) {
    json t;
    t["id"] = a.id;
    t["agent_name"] = a.agent_name;

    r.push_back(t);
  }

  j["status"] = "ok";
  j["result"] = r;

  return j.dump();
}

GetAgentsWithExistingDailyUserStatistics::GetAgentsWithExistingDailyUserStatistics(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
