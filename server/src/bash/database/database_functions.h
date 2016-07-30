/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef DATABASE_FUNCTIONS_H
#define DATABASE_FUNCTIONS_H

#include "detail/database_functions_interface.h"

#include "src/bash/database/detail/raw_database_functions_interface.h"
#include "src/database/detail/general_database_functions_interface.h"
#include "src/database/detail/sqlite_wrapper_interface.h"

namespace bash
{

namespace database
{

class DatabaseFunctions;
typedef std::shared_ptr<DatabaseFunctions> DatabaseFunctionsPtr;

class DatabaseFunctions : public detail::DatabaseFunctionsInterface {
 public:
  virtual ~DatabaseFunctions() = default;

  static DatabaseFunctionsPtr Create(::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper,
                                     ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);

  static DatabaseFunctionsPtr Create(::bash::database::detail::RawDatabaseFunctionsInterfacePtr raw_database_functions,
                                     ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);

  void CreateTables() override;

  void AddSystemUser(type::UID uid) override;
  ::database::type::RowId GetSystemUserId(type::UID uid) override;
  ::bash::database::detail::entity::SystemUser GetSystemUserById(::database::type::RowId id) override;
  ::database::type::RowIds GetSystemUsersIdsFromLogs(::database::type::RowId agent_name_id) override;

  void AddCommand(const ::bash::database::type::CommandName &command) override;
  ::database::type::RowId GetCommandId(const ::bash::database::type::CommandName &command) override;
  ::database::type::RowIds GetAllCommandsIds() override;
  ::bash::database::type::CommandName GetCommandNameById(::database::type::RowId id) override;

  void AddLog(const ::type::BashLogEntry &log_entry) override;
  ::database::type::RowsCount CountCommandsForDailySystemStatistic(::database::type::RowId agent_name_id,
                                                                   ::database::type::RowId date_id,
                                                                   ::database::type::RowId command_id) override;
  ::database::type::RowsCount CountCommandsForUserDailyStatisticFromLogs(::database::type::RowId agent_name_id,
                                                                         ::database::type::RowId date_id,
                                                                         ::database::type::RowId user_id,
                                                                         ::database::type::RowId command_id) override;

  void AddDailySystemStatistic(const detail::entity::DailySystemStatistic &statistics) override;
  void AddDailySystemStatistics(const detail::entity::DailySystemStatistics &statistics) override;
  ::database::type::RowIds GetDateIdsWithoutCreatedDailySystemStatistic(::database::type::RowId agent_name_id) override;
  ::database::type::RowId GetDailyUserStatisticId(::database::type::RowId agent_name_id,
                                                  ::database::type::RowId user_id,
                                                  ::database::type::RowId date_id) override;
  void SetDailyUserStatisticsClassification(const ::database::type::RowIds &ids, ::database::type::Classification classification) override;
  void AddDailyUserCommandStatistic(const ::bash::database::detail::entity::DailyUserCommandStatistic &ucs) override;

  ::database::type::RowIds GetAgentIdsWithoutConfiguration() override;
  ::database::type::RowIds GetAgentsIdsWithConfiguration() override;
  void AddDailyUserStatistic(const ::bash::database::detail::entity::DailyUserStatistic &us) override;
  ::database::entity::AgentNames GetAgentsWithExistingDailyUserStatistics() override;
  ::database::type::RowsCount CountDailyUserStatisticsForAgentWithClassification(::database::type::RowId agent_name_id,
                                                                                 ::database::type::Classification classification) override;
  ::bash::database::detail::entity::DailyUserStatistics GetDailyUserStatisticsForAgentWithClassification(::database::type::RowId agent_name_id,
                                                                                                         ::database::type::Classification classification,
                                                                                                         ::database::type::RowsCount limit,
                                                                                                         ::database::type::RowsCount offset) override;

  type::AnomalyDetectionConfigurations GetAnomalyDetectionConfigurations() override;
  void RemoveAnomalyDetectionConfiguration(::database::type::RowId id) override;
  void AddAnomalyDetectionConfiguration(const type::AnomalyDetectionConfiguration &configuration) override;
  ::database::type::RowId GetConfigurationIdForAgent(::database::type::RowId agent_id) override;
  void RemoveAllCommandsFromConfiguration(::database::type::RowId configuration_id) override;
  void AddDefaultCommandsToConfiguration(::database::type::RowId configuration_id) override;
  void MarkConfigurationAsUnchanged(::database::type::RowId configuration_id) override;
  void MarkConfigurationAsChanged(::database::type::RowId configuration_id) override;

  void AddCommandStatistic(const detail::entity::CommandStatistic &statistic) override;
  void AddCommandsStatistics(const detail::entity::CommandsStatistics &statistics) override;
  bool IsCommandStatisticExist(::database::type::RowId agent_name_id,
                               ::database::type::RowId command_id,
                               ::database::type::RowId begin_date_id,
                               ::database::type::RowId end_date_id) override;
  detail::entity::CommandsStatistics GetCommandsStatistics(::database::type::RowId agent_name_id,
                                                           ::database::type::RowId begin_date_id,
                                                           ::database::type::RowId end_date_id) override;
  detail::entity::CommandsStatistics GetCommandsStatistics(::database::type::RowId configuration_id) override;
  ::database::type::RowIds GetMarkedCommandsIds(::database::type::RowId configuration_id) override;
  ::database::type::RowsCount CommandSummary(::database::type::RowId command_id,
                                             ::database::type::RowIds date_range_ids) override;
  void AddSelectedCommandsIds(::database::type::RowId configuration_id,
                              ::database::type::RowIds command_names_ids) override;

  ::database::type::RowIds GetNotCalculatedDatesIdsFromLogs(::database::type::RowId agent_name_id,
                                                            ::database::type::RowId user_id) override;
  ::database::type::RowIds GetCommandsIdsFromLogs(::database::type::RowId agent_name_id,
                                                  ::database::type::RowId user_id,
                                                  ::database::type::RowId date_id) override;

  void AddDailyUserStatisticsToConfiguration(::database::type::RowId configuration_id,
                                             const ::database::type::RowIds &date_range_ids) override;
  void RemoveDailyStatisticsFromConfiguration(::database::type::RowId configuration_id) override;
  ::bash::database::detail::entity::DailyUserStatistics GetDailyUserStatisticsFromConfiguration(::database::type::RowId configuration_id) override;

  ::bash::database::detail::entity::DailyUserStatistics GetDailyUserStatisticsForAgent(::database::type::RowId agent_name_id,
                                                                                       const ::database::type::RowIds &date_range_ids) override;

  ::database::type::RowIds GetUsersIdsFromSelectedDailyStatisticsInConfiguration(::database::type::RowId agent_name_id) override;
  ::database::type::RowId CountSelectedDailyStatisticsWithoutUnknownClassificationInConfiguration(::database::type::RowId configuration_id) override;
  ::database::type::RowsCount CountSelectedDailyUserStatisticsWithoutUnknownClassificationFromConfigurationByUser(::database::type::RowId configuration_id,
                                                                                                                  ::database::type::RowId user_id) override;
  ::bash::database::detail::entity::DailyUserStatistics GetSelectedDailyUserStatisticsWithoutUnknownClassificationFromConfigurationByUser(::database::type::RowId configuration_id,
                                                                                                                                          ::database::type::RowId user_id,
                                                                                                                                          ::database::type::RowsCount limit,
                                                                                                                                          ::database::type::RowsCount offset) override;
  ::bash::database::detail::entity::DailyUserCommandsStatistics GetSelectedDailyUserCommandsStatistics(::database::type::RowId statistic_id) override;

 private:
  ::bash::database::detail::RawDatabaseFunctionsInterfacePtr raw_database_functions_;
  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;

  DatabaseFunctions(::bash::database::detail::RawDatabaseFunctionsInterfacePtr raw_database_functions,
                    ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);
};

}

}

#endif /* DATABASE_FUNCTIONS_H */
