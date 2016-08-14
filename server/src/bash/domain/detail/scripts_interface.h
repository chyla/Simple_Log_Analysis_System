/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include <slas/type/bash_log_entry.h>
#include <slas/type/date.h>

#include "src/bash/domain/type/command_statistic.h"
#include "src/bash/domain/type/unconfigured_agent.h"
#include "src/bash/domain/type/anomaly_detection_configuration.h"
#include "src/bash/domain/type/daily_user_statistic.h"
#include "src/database/entity/agent_name.h"

#include <memory>

namespace bash
{

namespace domain
{

namespace detail
{

class ScriptsInterface {
 public:
  virtual ~ScriptsInterface() = default;

  virtual void AddLog(const ::type::BashLogEntry &log_entry) = 0;

  virtual void CreateDailySystemStatistics() = 0;

  virtual ::bash::domain::type::UnconfiguredAgents GetUnconfigurentAgents() = 0;

  virtual ::bash::domain::type::AnomalyDetectionConfigurations GetAnomalyDetectionConfigurations() = 0;
  virtual void RemoveAnomalyDetectionConfiguration(::database::type::RowId id) = 0;
  virtual void SaveConfiguration(::database::type::RowId agent_name_id,
                                 const ::type::Date &begin_date,
                                 const ::type::Date &end_date) = 0;
  virtual ::database::type::RowId GetConfigurationIdForAgent(::database::type::RowId agent_id) = 0;
  virtual void AddDefaultCommandsToAgentConfiguration(::database::type::RowId agent_name_id) = 0;
  virtual void RemoveAllCommandsFromAgentConfiguration(::database::type::RowId agent_name_id) = 0;
  virtual void RemoveAllCommandsFromConfiguration(::database::type::RowId configuration_id) = 0;
  virtual void AddDailyUserStatisticsToConfiguration(::database::type::RowId agent_name_id,
                                                     const ::type::Date &begin_date,
                                                     const ::type::Date &end_date) = 0;
  virtual void RemoveDailyStatisticsFromConfiguration(::database::type::RowId configuration_id) = 0;
  virtual ::bash::domain::type::DailyUserStatistics GetDailyUserStatisticsFromConfiguration(::database::type::RowId configuration_id) = 0;
  virtual ::bash::domain::type::DailyUserStatistics GetDailyUserStatisticsForAgent(::database::type::RowId agent_name_id,
                                                                                   const ::type::Date &begin_date,
                                                                                   const ::type::Date &end_date) = 0;
  virtual ::database::entity::AgentNames GetAgentsWithExistingDailyUserStatistics() = 0;
  virtual void UpdateDailyUserStatisticsClassification(::database::type::RowId configuration_id,
                                                       ::database::type::RowIds normal_ids,
                                                       ::database::type::RowIds anomaly_ids) = 0;

  virtual void CalculateCommandStatistics(::database::type::RowId agent_name_id,
                                          const ::type::Date &begin_date,
                                          const ::type::Date &end_date) = 0;

  virtual ::bash::domain::type::CommandsStatistics GetCommandsStatistics(::database::type::RowId agent_name_id,
                                                                         const ::type::Date &begin_date,
                                                                         const ::type::Date &end_date) = 0;
  virtual ::bash::domain::type::CommandsStatistics GetCommandsStatistics(::database::type::RowId configuration_id) = 0;
  virtual ::database::type::RowIds GetMarkedCommandsIds(::database::type::RowId configuration_id) = 0;
  virtual void SaveSelectedCommands(::database::type::RowId configuration_id, ::database::type::RowIds command_names_ids) = 0;
  virtual void SelectDefaultCommands(::database::type::RowId configuration_id) = 0;
};

typedef std::shared_ptr<ScriptsInterface> ScriptsInterfacePtr;

}

}

}
