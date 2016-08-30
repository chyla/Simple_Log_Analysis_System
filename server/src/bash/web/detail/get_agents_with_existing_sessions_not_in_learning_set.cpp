/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "get_agents_with_existing_sessions_not_in_learning_set.h"

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

GetAgentsWithExistingSessionsNotInLearningSetPtr GetAgentsWithExistingSessionsNotInLearningSet::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return GetAgentsWithExistingSessionsNotInLearningSetPtr(new GetAgentsWithExistingSessionsNotInLearningSet(scripts));
}

::web::type::Command GetAgentsWithExistingSessionsNotInLearningSet::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetAgentsWithExistingSessionsNotInLearningSet::GetCommandName: Function call";

  return "bash.get_agents_with_existing_sessions_not_in_learning_set";
}

::web::type::JsonMessage GetAgentsWithExistingSessionsNotInLearningSet::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetAgentsWithExistingSessionsNotInLearningSet::Execute: Function call";

  auto agents = scripts_->GetAgentsWithExistingDailyUserStatisticsNotInLearningSet();

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

GetAgentsWithExistingSessionsNotInLearningSet::GetAgentsWithExistingSessionsNotInLearningSet(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
