/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "select_default_commands.h"

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

SelectDefaultCommandsPtr SelectDefaultCommands::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return SelectDefaultCommandsPtr(new SelectDefaultCommands(scripts));
}

::web::type::Command SelectDefaultCommands::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetUnconfiguredAgents::GetCommandName: Function call";

  return "bash.select_default_commands";
}

::web::type::JsonMessage SelectDefaultCommands::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::SelectDefaultCommands::Execute: Function call";

  auto json_object = json::parse(message);
  auto args = json_object["args"];

  scripts_->SelectDefaultCommands(args.at(0));

  json j;
  j["status"] = "ok";

  return j.dump();
}

SelectDefaultCommands::SelectDefaultCommands(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
