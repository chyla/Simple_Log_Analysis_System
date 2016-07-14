/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "get_commands_statistics_by_configuration_id.h"

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

GetCommandsStatisticsByConfigurationIdPtr GetCommandsStatisticsByConfigurationId::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return GetCommandsStatisticsByConfigurationIdPtr(new GetCommandsStatisticsByConfigurationId(scripts));
}

::web::type::Command GetCommandsStatisticsByConfigurationId::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetCommandsStatisticsByConfigurationId::GetCommandName: Function call";

  return "bash.get_commands_statistics_by_configuration_id";
}

::web::type::JsonMessage GetCommandsStatisticsByConfigurationId::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetCommandsStatisticsByConfigurationId::Execute: Function call";

  auto json_object = json::parse(message);
  auto args = json_object["args"];

  auto statistics = scripts_->GetCommandsStatistics(args.at(0));

  json j, r = json::array();
  for (const auto &s : statistics) {
    json t;
    t["command_name_id"] = s.command_name_id;
    t["command_name"] = s.command_name;
    t["summary_count"] = s.summary_count;

    r.push_back(t);
  }

  j["status"] = "ok";
  j["result"] = r;

  return j.dump();
}

GetCommandsStatisticsByConfigurationId::GetCommandsStatisticsByConfigurationId(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
