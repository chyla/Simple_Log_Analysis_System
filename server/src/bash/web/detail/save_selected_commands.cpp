/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "save_selected_commands.h"

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

SaveSelectedCommandsPtr SaveSelectedCommands::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  return SaveSelectedCommandsPtr(new SaveSelectedCommands(scripts));
}

::web::type::Command SaveSelectedCommands::GetCommandName() {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::GetUnconfiguredAgents::GetCommandName: Function call";

  return "bash.save_selected_commands";
}

::web::type::JsonMessage SaveSelectedCommands::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::detail::SaveSelectedCommands::Execute: Function call";

  auto json_object = json::parse(message);
  auto args = json_object["args"];

  scripts_->SaveSelectedCommands(args.at(0), args.at(1));

  json j;
  j["status"] = "ok";

  return j.dump();
}

SaveSelectedCommands::SaveSelectedCommands(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}

}
