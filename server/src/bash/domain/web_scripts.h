/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "detail/scripts_interface.h"
#include "detail/web_scripts_interface.h"

namespace bash
{

namespace domain
{

class WebScripts;
typedef std::shared_ptr<WebScripts> WebScriptsPtr;

class WebScripts : public detail::WebScriptsInterface {
 public:
  virtual ~WebScripts() = default;

  static WebScriptsPtr Create(::bash::domain::detail::ScriptsInterfacePtr scripts);

  ::bash::domain::type::UnconfiguredAgents GetUnconfigurentAgents() override;

  ::bash::domain::type::AnomalyDetectionConfigurations GetAnomalyDetectionConfigurations() override;
  void RemoveAnomalyDetectionConfiguration(::database::type::RowId id) override;
  void SaveConfiguration(::database::type::RowId agent_name_id,
                         const ::type::Date &begin_date,
                         const ::type::Date &end_date) override;
  ::database::type::RowId GetConfigurationIdForAgent(::database::type::RowId agent_id) override;
  ::bash::domain::type::DailyUserStatistics GetDailyUserStatisticsForAgent(::database::type::RowId agent_name_id,
                                                                           const ::type::Date &begin_date,
                                                                           const ::type::Date &end_date) override;
  ::bash::domain::type::DailyUserStatistic GetDailyUserStatisticById(::database::type::RowId id) override;
  ::database::entity::AgentNames GetAgentsWithExistingDailyUserStatistics() override;

  ::bash::domain::type::DailyUserStatistics GetDailyUserStatisticsFromConfiguration(::database::type::RowId configuration_id) override;
  void UpdateDailyUserStatisticsClassification(::database::type::RowId configuration_id,
                                               ::database::type::RowIds normal_ids,
                                               ::database::type::RowIds anomaly_ids) override;

  ::bash::domain::type::CommandsStatistics GetCommandsStatistics(::database::type::RowId agent_name_id,
                                                                 const ::type::Date &begin_date,
                                                                 const ::type::Date &end_date) override;
  ::bash::domain::type::CommandsStatistics GetCommandsStatistics(::database::type::RowId configuration_id) override;
  ::database::type::RowIds GetMarkedCommandsIds(::database::type::RowId configuration_id) override;
  void SaveSelectedCommands(::database::type::RowId configuration_id, ::database::type::RowIds command_names_ids) override;
  void SelectDefaultCommands(::database::type::RowId configuration_id) override;

  ::bash::database::detail::type::DailyUserNamedCommandsStatistics GetDailyUserNamedCommandsStatistics(::database::type::RowId statistic_id) override;
 private:
  ::bash::domain::detail::ScriptsInterfacePtr scripts_;

  WebScripts(::bash::domain::detail::ScriptsInterfacePtr scripts);
};

}

}
