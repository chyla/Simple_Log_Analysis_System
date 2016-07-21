/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "get_daily_user_statistics_from_configuration.h"
#include "src/bash/domain/type/daily_user_statistic.h"

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

GetDailyUserStatisticsFromConfigurationPtr GetDailyUserStatisticsFromConfiguration::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return GetDailyUserStatisticsFromConfigurationPtr(new GetDailyUserStatisticsFromConfiguration(scripts));
}

::web::type::Command GetDailyUserStatisticsFromConfiguration::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetDailyUserStatisticsFromConfiguration::GetCommandName: Function call";

  return "bash.get_daily_user_statistics_from_configuration";
}

::web::type::JsonMessage GetDailyUserStatisticsFromConfiguration::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetDailyUserStatisticsFromConfiguration::Execute: Function call";

  auto json_object = json::parse(message);
  auto args = json_object["args"];

  auto statistics = scripts_->GetDailyUserStatisticsFromConfiguration(args.at(0));

  json j, r = json::array();
  for (const auto &s : statistics) {
    json t;
    t["id"] = s.id;
    t["agent_name"] = s.agent_name;
    t["uid"] = s.uid;
    t["date"] = s.date.ToString();
    t["classification"] = static_cast<int>(s.classification);

    r.push_back(t);
  }

  j["status"] = "ok";
  j["result"] = r;

  return j.dump();
}

GetDailyUserStatisticsFromConfiguration::GetDailyUserStatisticsFromConfiguration(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
