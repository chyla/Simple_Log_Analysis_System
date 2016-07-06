/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "command_executor_object.h"

#include "detail/get_anomaly_detection_configuration.h"
#include "detail/get_unconfigured_agents.h"
#include "detail/remove_anomaly_detection_configuration.h"
#include "detail/get_commands_statistics.h"
#include "detail/save_anomaly_detection_configuration.h"

#include <boost/log/trivial.hpp>
#include <json/json.hpp>

using namespace std;
using namespace nlohmann;

namespace bash
{

namespace web
{

CommandExecutorObjectPtr CommandExecutorObject::Create(::bash::domain::detail::WebScriptsInterfacePtr scripts) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::CommandExecutorObject::Create: Function call";
  return CommandExecutorObjectPtr(new CommandExecutorObject(scripts));
}

const ::web::type::JsonMessage CommandExecutorObject::Execute(const ::web::type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::CommandExecutorObject::Execute: Function call";

  auto json_object = json::parse(message);
  auto command = json_object["command"];
  auto result = GetUnknownCommandErrorJson();

  auto it = commands_.find(command);
  if (it != commands_.end())
    result = it->second->Execute(message);

  return result;
}

bool CommandExecutorObject::IsCommandSupported(const ::web::type::Command &command) {
  BOOST_LOG_TRIVIAL(debug) << "bash::web::CommandExecutorObject::IsCommandSupported: Function call";

  auto it = commands_.find(command);

  return it != commands_.end();
}

CommandExecutorObject::CommandExecutorObject(::bash::domain::detail::WebScriptsInterfacePtr scripts) :
scripts_(scripts) {
  std::vector<detail::WebCommandPtr> web_commands{
    detail::GetAnomalyDetectionConfiguration::Create(scripts),
    detail::GetUnconfiguredAgents::Create(scripts),
    detail::RemoveAnomalyDetectionConfiguration::Create(scripts),
    detail::GetCommandsStatistics::Create(scripts),
    detail::SaveAnomalyDetectionConfiguration::Create(scripts)
  };

  for (auto &c : web_commands)
    commands_[c->GetCommandName()] = c;
}

}

}
