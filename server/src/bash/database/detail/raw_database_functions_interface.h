/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef RAW_DATABASE_FUNCTIONS_INTERFACE_H
#define RAW_DATABASE_FUNCTIONS_INTERFACE_H

#include "entity/anomaly_detection_configuration.h"
#include "entity/daily_system_statistic.h"
#include "entity/log.h"
#include "entity/system_user.h"
#include "src/bash/database/type/command_name.h"
#include "entity/command_statistic.h"
#include "src/bash/database/detail/entity/daily_user_statistic.h"
#include "src/bash/database/detail/entity/daily_user_command_statistic.h"
#include "src/bash/database/detail/entity/system_user.h"
#include "src/database/entity/agent_name.h"

#include <memory>

namespace bash
{

namespace database
{

namespace detail
{

class RawDatabaseFunctionsInterface {
 public:
  virtual ~RawDatabaseFunctionsInterface() = default;

  virtual void CreateTables() = 0;

  virtual void AddSystemUser(const entity::SystemUser &system_user) = 0;
  virtual ::database::type::RowId GetSystemUserId(const entity::SystemUser &system_user) = 0;
  virtual ::bash::database::detail::entity::SystemUser GetSystemUserById(::database::type::RowId id) = 0;
  virtual ::database::type::RowIds GetSystemUsersIdsFromLogs(::database::type::RowId agent_name_id) = 0;

  virtual void AddCommand(const ::bash::database::type::CommandName &command) = 0;
  virtual ::database::type::RowId GetCommandId(const ::bash::database::type::CommandName &command) = 0;
  virtual ::database::type::RowIds GetAllCommandsIds() = 0;
  virtual ::bash::database::type::CommandName GetCommandNameById(::database::type::RowId id) = 0;

  virtual void AddLog(const entity::Log &log) = 0;
  virtual ::database::type::RowsCount CountCommandsForDailySystemStatistic(::database::type::RowId agent_name_id,
                                                                           ::database::type::RowId date_id,
                                                                           ::database::type::RowId command_id) = 0;
  virtual ::database::type::RowsCount CountCommandsForUserDailyStatisticFromLogs(::database::type::RowId agent_name_id,
                                                                                 ::database::type::RowId date_id,
                                                                                 ::database::type::RowId user_id,
                                                                                 ::database::type::RowId command_id) = 0;

  virtual void AddDailySystemStatistic(const entity::DailySystemStatistic &statistics) = 0;
  virtual ::database::type::RowIds GetDateIdsWithoutCreatedDailySystemStatistic(::database::type::RowId agent_name_id) = 0;

  virtual ::database::type::RowIds GetAgentIdsWithoutConfiguration() = 0;
  virtual ::database::type::RowIds GetAgentsIdsWithConfiguration() = 0;
  virtual void AddDailyUserStatistic(const ::bash::database::detail::entity::DailyUserStatistic &us) = 0;
  virtual ::database::type::RowId GetDailyUserStatisticId(::database::type::RowId agent_name_id,
                                                          ::database::type::RowId user_id,
                                                          ::database::type::RowId date_id) = 0;
  virtual void AddDailyUserCommandStatistic(const ::bash::database::detail::entity::DailyUserCommandStatistic &ucs) = 0;
  virtual ::database::entity::AgentNames GetAgentsWithExistingDailyUserStatistics() = 0;

  virtual entity::AnomalyDetectionConfigurations GetAnomalyDetectionConfigurations() = 0;
  virtual void RemoveAnomalyDetectionConfiguration(::database::type::RowId id) = 0;
  virtual void AddAnomalyDetectionConfiguration(const entity::AnomalyDetectionConfiguration &configuration) = 0;
  virtual ::database::type::RowId GetConfigurationIdForAgent(::database::type::RowId agent_id) = 0;
  virtual void RemoveAllCommandsFromConfiguration(::database::type::RowId configuration_id) = 0;
  virtual void AddDefaultCommandsToConfiguration(::database::type::RowId configuration_id) = 0;
  virtual void MarkConfigurationAsUnchanged(::database::type::RowId configuration_id) = 0;

  virtual void AddCommandStatistic(const entity::CommandStatistic &statistic) = 0;
  virtual bool IsCommandStatisticExist(::database::type::RowId agent_name_id,
                                       ::database::type::RowId command_id,
                                       ::database::type::RowId begin_date_id,
                                       ::database::type::RowId end_date_id) = 0;
  virtual entity::CommandsStatistics GetCommandsStatistics(::database::type::RowId agent_name_id,
                                                           ::database::type::RowId begin_date_id,
                                                           ::database::type::RowId end_date_id) = 0;
  virtual entity::CommandsStatistics GetCommandsStatistics(::database::type::RowId configuration_id) = 0;
  virtual ::database::type::RowIds GetMarkedCommandsIds(::database::type::RowId configuration_id) = 0;
  virtual void AddSelectedCommandsIds(::database::type::RowId configuration_id,
                                      ::database::type::RowIds command_names_ids) = 0;
  virtual ::database::type::RowsCount CommandSummary(::database::type::RowId command_id,
                                                     ::database::type::RowIds date_range_ids) = 0;

  virtual ::database::type::RowIds GetNotCalculatedDatesIdsFromLogs(::database::type::RowId agent_name_id,
                                                                    ::database::type::RowId user_id) = 0;
  virtual ::database::type::RowIds GetCommandsIdsFromLogs(::database::type::RowId agent_name_id,
                                                          ::database::type::RowId user_id,
                                                          ::database::type::RowId date_id) = 0;

  virtual void AddDailyUserStatisticsToConfiguration(::database::type::RowId configuration_id,
                                                     const ::database::type::RowIds &date_range_ids) = 0;
  virtual void RemoveDailyStatisticsFromConfiguration(::database::type::RowId configuration_id) = 0;
  virtual ::bash::database::detail::entity::DailyUserStatistics GetDailyUserStatisticsFromConfiguration(::database::type::RowId configuration_id) = 0;

  virtual ::bash::database::detail::entity::DailyUserStatistics GetDailyUserStatisticsForAgent(::database::type::RowId agent_name_id,
                                                                                               const ::database::type::RowIds &date_range_ids) = 0;

  virtual ::database::type::RowIds GetUsersIdsFromSelectedDailyStatisticsInConfiguration(::database::type::RowId configuration_id) = 0;
  virtual ::database::type::RowId GetNumberOfSelectedDailyStatisticsInConfiguration(::database::type::RowId configuration_id) = 0;
  virtual ::database::type::RowsCount GetSelectedDailyUserStatisticsCountFromConfigurationByUser(::database::type::RowId configuration_id,
                                                                                                 ::database::type::RowId user_id) = 0;
  virtual ::bash::database::detail::entity::DailyUserStatistics GetSelectedDailyUserStatisticsFromConfigurationByUser(::database::type::RowId configuration_id,
                                                                                                                      ::database::type::RowId user_id,
                                                                                                                      ::database::type::RowsCount limit,
                                                                                                                      ::database::type::RowsCount offset) = 0;
  virtual ::bash::database::detail::entity::DailyUserCommandsStatistics GetSelectedDailyUserCommandsStatistics(::database::type::RowId statistic_id) = 0;
};

typedef std::shared_ptr<RawDatabaseFunctionsInterface> RawDatabaseFunctionsInterfacePtr;

}

}

}

#endif /* RAW_DATABASE_FUNCTIONS_INTERFACE_H */
