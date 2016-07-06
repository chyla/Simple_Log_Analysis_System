/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "web_scripts.h"

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

  scripts_->RemoveAnomalyDetectionConfiguration(id);
}

void WebScripts::SaveConfiguration(::database::type::RowId agent_name_id,
                                   const ::type::Date &begin_date,
                                   const ::type::Date &end_date) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::WebScripts::SaveConfiguration: Function call";

  scripts_->SaveConfiguration(agent_name_id, begin_date, end_date);
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

WebScripts::WebScripts(::bash::domain::detail::ScriptsInterfacePtr scripts) :
scripts_(scripts) {
}

}

}
