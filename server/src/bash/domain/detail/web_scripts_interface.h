/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef WEB_SCRIPTS_INTERFACE_H
#define WEB_SCRIPTS_INTERFACE_H

#include "src/database/type/row_id.h"
#include "src/database/type/rows_count.h"
#include "src/bash/domain/type/command_statistic.h"
#include "src/bash/domain/type/unconfigured_agent.h"
#include "src/bash/domain/type/anomaly_detection_configuration.h"

#include <memory>

namespace bash
{

namespace domain
{

namespace detail
{

class WebScriptsInterface {
 public:
  virtual ~WebScriptsInterface() = default;

  virtual ::bash::domain::type::UnconfiguredAgents GetUnconfigurentAgents() = 0;

  virtual ::bash::domain::type::AnomalyDetectionConfigurations GetAnomalyDetectionConfigurations() = 0;
  virtual void RemoveAnomalyDetectionConfiguration(::database::type::RowId id) = 0;
  virtual void SaveConfiguration(::database::type::RowId agent_name_id,
                                 const ::type::Date &begin_date,
                                 const ::type::Date &end_date) = 0;
  virtual ::database::type::RowId GetConfigurationIdForAgent(::database::type::RowId agent_id) = 0;

  virtual ::bash::domain::type::CommandsStatistics GetCommandsStatistics(::database::type::RowId agent_name_id,
                                                                         const ::type::Date &begin_date,
                                                                         const ::type::Date &end_date) = 0;
  virtual ::bash::domain::type::CommandsStatistics GetCommandsStatistics(::database::type::RowId configuration_id) = 0;
  virtual ::database::type::RowIds GetMarkedCommandsIds(::database::type::RowId configuration_id) = 0;
  virtual void SaveSelectedCommands(::database::type::RowId configuration_id, ::database::type::RowIds command_names_ids) = 0;
  virtual void SelectDefaultCommands(::database::type::RowId configuration_id) = 0;
};

typedef std::shared_ptr<WebScriptsInterface> WebScriptsInterfacePtr;

}

}

}

#endif /* WEB_SCRIPTS_INTERFACE_H */
