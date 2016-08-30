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
#include "detail/get_configuration_id_for_agent.h"
#include "detail/get_commands_statistics_by_configuration_id.h"
#include "detail/get_selected_commands_ids.h"
#include "detail/save_selected_commands.h"
#include "detail/select_default_commands.h"
#include "detail/get_daily_user_statistics_from_configuration.h"
#include "detail/get_daily_user_statistics_for_agent.h"
#include "detail/get_agents_with_existing_daily_user_statistics.h"
#include "detail/set_sessions_as_anomalies.h"
#include "detail/get_daily_user_statistic_by_id.h"
#include "detail/get_daily_user_named_commands_statistics.h"
#include "detail/get_daily_user_statistics_without_learning_set_for_agent.h"
#include "detail/get_agents_with_existing_sessions_not_in_learning_set.h"

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
    detail::SaveAnomalyDetectionConfiguration::Create(scripts),
    detail::GetConfigurationIdForAgent::Create(scripts),
    detail::GetCommandsStatisticsByConfigurationId::Create(scripts),
    detail::GetSelectedCommandsIds::Create(scripts),
    detail::SaveSelectedCommands::Create(scripts),
    detail::SelectDefaultCommands::Create(scripts),
    detail::GetDailyUserStatisticsFromConfiguration::Create(scripts),
    detail::GetDailyUserStatisticsForAgent::Create(scripts),
    detail::GetAgentsWithExistingDailyUserStatistics::Create(scripts),
    detail::SetSessionsAsAnomalies::Create(scripts),
    detail::GetDailyUserStatisticById::Create(scripts),
    detail::GetDailyUserNamedCommandsStatistics::Create(scripts),
    detail::GetDailyUserStatisticsWithoutLearningSetForAgent::Create(scripts),
    detail::GetAgentsWithExistingSessionsNotInLearningSet::Create(scripts)
  };

  for (auto &c : web_commands)
    commands_[c->GetCommandName()] = c;
}

}

}
