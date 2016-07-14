/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "get_selected_commands_ids.h"

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

GetSelectedCommandsIdsPtr GetSelectedCommandsIds::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return GetSelectedCommandsIdsPtr(new GetSelectedCommandsIds(scripts));
}

::web::type::Command GetSelectedCommandsIds::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetMarkedCommandsIds::GetCommandName: Function call";

  return "bash.get_selected_commands_ids";
}

::web::type::JsonMessage GetSelectedCommandsIds::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetMarkedCommandsIds::Execute: Function call";

  auto json_object = json::parse(message);
  auto args = json_object["args"];

  auto statistics = scripts_->GetMarkedCommandsIds(args.at(0));

  json j, r = json::array();
  for (const auto &s : statistics)
    r.push_back(s);

  j["status"] = "ok";
  j["result"] = r;

  return j.dump();
}

GetSelectedCommandsIds::GetSelectedCommandsIds(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
