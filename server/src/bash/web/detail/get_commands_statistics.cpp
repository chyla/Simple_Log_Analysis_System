/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "get_commands_statistics.h"
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

GetCommandsStatisticsPtr GetCommandsStatistics::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return GetCommandsStatisticsPtr(new GetCommandsStatistics(scripts));
}

::web::type::Command GetCommandsStatistics::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetCommandsStatistics::GetCommandName: Function call";

  return "bash.get_commands_statistics";
}

::web::type::JsonMessage GetCommandsStatistics::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetCommandsStatistics::Execute: Function call";

  auto json_object = json::parse(message);
  auto args = json_object["args"];

  auto statistics = scripts_->GetCommandsStatistics(args.at(0),
                                                    ::type::Date::Create(args.at(1)),
                                                    ::type::Date::Create(args.at(2)));

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

GetCommandsStatistics::GetCommandsStatistics(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
