/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "web_scripts.h"
#include "scripts.h"

#include <boost/log/trivial.hpp>

namespace bash
{

namespace domain
{

WebScriptsPtr WebScripts::Create(::bash::domain::detail::ScriptsInterfacePtr scripts) {
  return WebScriptsPtr(new WebScripts(scripts));
}

::bash::domain::type::UnconfiguredAgents WebScripts::GetUnconfigurentAgents() {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::WebScripts::GetUnconfigurentAgents: Function call";

  return scripts_->GetUnconfigurentAgents();
}

::bash::domain::type::AnomalyDetectionConfigurations WebScripts::GetAnomalyDetectionConfigurations() {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::WebScripts::GetAnomalyDetectionConfigurations: Function call";

  return scripts_->GetAnomalyDetectionConfigurations();
}

void WebScripts::RemoveAnomalyDetectionConfiguration(::database::type::RowId id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::WebScripts::RemoveAnomalyDetectionConfiguration: Function call";

  scripts_->RemoveAllCommandsFromConfiguration(id);

  scripts_->RemoveAnomalyDetectionConfiguration(id);
}

void WebScripts::SaveConfiguration(::database::type::RowId agent_name_id,
                                   const ::type::Date &begin_date,
                                   const ::type::Date &end_date) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::WebScripts::SaveConfiguration: Function call";

  scripts_->SaveConfiguration(agent_name_id, begin_date, end_date);

  scripts_->RemoveAllCommandsFromAgentConfiguration(agent_name_id);

  scripts_->AddDefaultCommandsToAgentConfiguration(agent_name_id);
}

::database::type::RowId WebScripts::GetConfigurationIdForAgent(::database::type::RowId agent_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::WebScripts::GetConfigurationIdForAgent: Function call";

  return scripts_->GetConfigurationIdForAgent(agent_id);
}

::bash::domain::type::CommandsStatistics WebScripts::GetCommandsStatistics(::database::type::RowId agent_name_id,
                                                                           const ::type::Date &begin_date,
                                                                           const ::type::Date &end_date) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::WebScripts::GetCommandsStatistics: Function call";

  BOOST_LOG_TRIVIAL(debug) << "bash::domain::WebScripts::GetCommandsStatistics: Creating command statistics";
  scripts_->CalculateCommandStatistics(agent_name_id, begin_date, end_date);

  BOOST_LOG_TRIVIAL(debug) << "bash::domain::WebScripts::GetCommandsStatistics: Returning statistics";
  auto statistics = scripts_->GetCommandsStatistics(agent_name_id, begin_date, end_date);

  return statistics;
}

::bash::domain::type::CommandsStatistics WebScripts::GetCommandsStatistics(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::WebScripts::GetCommandsStatistics: Function call";

  BOOST_LOG_TRIVIAL(debug) << "bash::domain::WebScripts::GetCommandsStatistics: Returning statistics";
  auto statistics = scripts_->GetCommandsStatistics(configuration_id);

  return statistics;
}

::database::type::RowIds WebScripts::GetMarkedCommandsIds(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::WebScripts::GetMarkedCommandsIds: Function call";

  return scripts_->GetMarkedCommandsIds(configuration_id);
}

void WebScripts::SaveSelectedCommands(::database::type::RowId configuration_id, ::database::type::RowIds command_names_ids) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::WebScripts::SaveSelectedCommands: Function call";

  scripts_->SaveSelectedCommands(configuration_id, command_names_ids);
}

void WebScripts::SelectDefaultCommands(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::WebScripts::SelectDefaultCommands: Function call";

  scripts_->SelectDefaultCommands(configuration_id);
}

WebScripts::WebScripts(::bash::domain::detail::ScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}
