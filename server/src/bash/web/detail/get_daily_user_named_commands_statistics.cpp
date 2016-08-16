/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "get_daily_user_named_commands_statistics.h"
#include "src/bash/domain/detail/web_scripts_interface.h"

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

GetDailyUserNamedCommandsStatisticsPtr GetDailyUserNamedCommandsStatistics::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return GetDailyUserNamedCommandsStatisticsPtr(new GetDailyUserNamedCommandsStatistics(scripts));
}

::web::type::Command GetDailyUserNamedCommandsStatistics::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetDailyUserNamedCommandsStatistics::GetCommandName: Function call";

  return "bash.get_daily_user_named_commands_statistics";
}

::web::type::JsonMessage GetDailyUserNamedCommandsStatistics::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetDailyUserNamedCommandsStatistics::Execute: Function call";

  auto json_object = json::parse(message);
  auto args = json_object["args"];

  auto statistics = scripts_->GetDailyUserNamedCommandsStatistics(args.at(0));

  json j, r = json::array();
  for (const auto &s : statistics) {
    json t;
    t["command_id"] = s.command_id;
    t["command_name"] = s.command_name;
    t["summary"] = s.summary;

    r.push_back(t);
  }

  j["status"] = "ok";
  j["result"] = r;

  return j.dump();
}

GetDailyUserNamedCommandsStatistics::GetDailyUserNamedCommandsStatistics(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
