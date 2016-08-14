/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "get_daily_user_statistics_for_agent.h"

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

GetDailyUserStatisticsForAgentPtr GetDailyUserStatisticsForAgent::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return GetDailyUserStatisticsForAgentPtr(new GetDailyUserStatisticsForAgent(scripts));
}

::web::type::Command GetDailyUserStatisticsForAgent::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetDailyUserStatisticsForAgent::GetCommandName: Function call";

  return "bash.get_daily_user_statistics_for_agent";
}

::web::type::JsonMessage GetDailyUserStatisticsForAgent::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetDailyUserStatisticsForAgent::Execute: Function call";

  auto json_object = json::parse(message);
  auto args = json_object["args"];

  auto begin_date = ::type::Date::Create(args.at(1));
  auto end_date = ::type::Date::Create(args.at(2));
  
  auto statistics = scripts_->GetDailyUserStatisticsForAgent(args.at(0), begin_date, end_date);

  json j, r = json::array();
  for (const auto &s : statistics) {
    json t;
    t["id"] = s.id;
    t["agent_name"] = s.agent_name;
    t["uid"] = s.uid;
    t["date"] = s.date.ToString();
    t["classification"] = static_cast<int> (s.classification);

    r.push_back(t);
  }

  j["status"] = "ok";
  j["result"] = r;

  return j.dump();
}

GetDailyUserStatisticsForAgent::GetDailyUserStatisticsForAgent(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
