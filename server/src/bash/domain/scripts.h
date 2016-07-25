/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SCRIPTS_H
#define SCRIPTS_H

#include "detail/scripts_interface.h"
#include "src/bash/database/detail/database_functions_interface.h"
#include "src/database/detail/general_database_functions_interface.h"

namespace bash
{

namespace domain
{

class Scripts;
typedef std::shared_ptr<Scripts> ScriptsPtr;

class Scripts : public detail::ScriptsInterface {
 public:
  virtual ~Scripts() = default;

  static ScriptsPtr Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                           ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);

  void AddLog(const ::type::BashLogEntry &log_entry) override;

  void CreateDailySystemStatistics() override;

  ::bash::domain::type::UnconfiguredAgents GetUnconfigurentAgents() override;

  ::bash::domain::type::AnomalyDetectionConfigurations GetAnomalyDetectionConfigurations() override;
  void RemoveAnomalyDetectionConfiguration(::database::type::RowId id) override;
  void SaveConfiguration(::database::type::RowId agent_name_id,
                         const ::type::Date &begin_date,
                         const ::type::Date &end_date) override;
  ::database::type::RowId GetConfigurationIdForAgent(::database::type::RowId agent_id) override;
  void AddDefaultCommandsToAgentConfiguration(::database::type::RowId agent_name_id) override;
  void RemoveAllCommandsFromAgentConfiguration(::database::type::RowId agent_name_id) override;
  void RemoveAllCommandsFromConfiguration(::database::type::RowId configuration_id) override;
  void AddDailyUserStatisticsToConfiguration(::database::type::RowId agent_name_id,
                                             const ::type::Date &begin_date,
                                             const ::type::Date &end_date) override;
  void RemoveDailyStatisticsFromConfiguration(::database::type::RowId configuration_id) override;
  ::bash::domain::type::DailyUserStatistics GetDailyUserStatisticsFromConfiguration(::database::type::RowId configuration_id) override;
  ::bash::domain::type::DailyUserStatistics GetDailyUserStatisticsForAgent(::database::type::RowId agent_name_id,
                                                                           const ::type::Date &begin_date,
                                                                           const ::type::Date &end_date) override;

  void CalculateCommandStatistics(::database::type::RowId agent_name_id,
                                  const ::type::Date &begin_date,
                                  const ::type::Date &end_date) override;

  ::bash::domain::type::CommandsStatistics GetCommandsStatistics(::database::type::RowId agent_name_id,
                                                                 const ::type::Date &begin_date,
                                                                 const ::type::Date &end_date) override;
  ::bash::domain::type::CommandsStatistics GetCommandsStatistics(::database::type::RowId configuration_id) override;
  ::database::type::RowIds GetMarkedCommandsIds(::database::type::RowId configuration_id) override;
  void SaveSelectedCommands(::database::type::RowId configuration_id, ::database::type::RowIds command_names_ids) override;
  void SelectDefaultCommands(::database::type::RowId configuration_id) override;

 private:
  ::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions_;
  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;

  Scripts(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
          ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);
};

}

}

#endif /* SCRIPTS_H */
