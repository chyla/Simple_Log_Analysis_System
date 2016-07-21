/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "database_functions.h"

#include <boost/log/trivial.hpp>
#include <string>

#include "type/uid.h"
#include "detail/raw_database_functions.h"

using namespace std;

namespace bash
{

namespace database
{

DatabaseFunctionsPtr DatabaseFunctions::Create(::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper,
                                               ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::Create: Function call";

  auto raw_database_functions = detail::RawDatabaseFunctions::Create(sqlite_wrapper);

  return Create(raw_database_functions, general_database_functions);
}

DatabaseFunctionsPtr DatabaseFunctions::Create(::bash::database::detail::RawDatabaseFunctionsInterfacePtr raw_database_functions,
                                               ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::Create: Function call";

  return DatabaseFunctionsPtr(new DatabaseFunctions(raw_database_functions, general_database_functions));
}

void DatabaseFunctions::CreateTables() {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::CreateTables: Function call";

  raw_database_functions_->CreateTables();
}

void DatabaseFunctions::AddSystemUser(type::UID uid) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::AddSystemUser: Function call";

  raw_database_functions_->AddSystemUser({uid});
}

::database::type::RowId DatabaseFunctions::GetSystemUserId(type::UID uid) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::GetSystemUserId: Function call";

  return raw_database_functions_->GetSystemUserId({uid});
}

::bash::database::detail::entity::SystemUser DatabaseFunctions::GetSystemUserById(::database::type::RowId id) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::GetSystemUserById: Function call";

  return raw_database_functions_->GetSystemUserById(id);
}

::database::type::RowIds DatabaseFunctions::GetSystemUsersIdsFromLogs(::database::type::RowId agent_name_id) {
  BOOST_LOG_TRIVIAL(debug) << "database::DatabaseFunctions::GetSystemUsersIdsFromLogs: Function call";

  return raw_database_functions_->GetSystemUsersIdsFromLogs(agent_name_id);
}

void DatabaseFunctions::AddCommand(const ::bash::database::type::CommandName &command) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::AddCommand: Function call";

  return raw_database_functions_->AddCommand(command);
}

::database::type::RowId DatabaseFunctions::GetCommandId(const ::bash::database::type::CommandName &command) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetCommandId: Function call";

  return raw_database_functions_->GetCommandId(command);
}

::database::type::RowIds DatabaseFunctions::GetAllCommandsIds() {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetAllCommandsIds: Function call";

  return raw_database_functions_->GetAllCommandsIds();
}

::bash::database::type::CommandName DatabaseFunctions::GetCommandNameById(::database::type::RowId id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetCommandNameById: Function call";

  return raw_database_functions_->GetCommandNameById(id);
}

void DatabaseFunctions::AddLog(const ::type::BashLogEntry &log_entry) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::AddLog: Function call";

  detail::entity::Log raw_log;
  raw_log.agent_name_id = general_database_functions_->GetAgentNameId(log_entry.agent_name);
  raw_log.date_id = general_database_functions_->GetDateId(log_entry.utc_time.GetDate());
  raw_log.time_id = general_database_functions_->GetTimeId(log_entry.utc_time.GetTime());
  raw_log.user_id = GetSystemUserId(log_entry.user_id);
  raw_log.command_id = raw_database_functions_->GetCommandId(log_entry.command);

  raw_database_functions_->AddLog(raw_log);
}

::database::type::RowsCount DatabaseFunctions::CountCommandsForDailySystemStatistic(::database::type::RowId agent_name_id,
                                                                                    ::database::type::RowId date_id,
                                                                                    ::database::type::RowId command_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::CountCommandsForDailySystemStatistic: Function call";

  return raw_database_functions_->CountCommandsForDailySystemStatistic(agent_name_id, date_id, command_id);
}

::database::type::RowsCount DatabaseFunctions::CountCommandsForUserDailyStatisticFromLogs(::database::type::RowId agent_name_id,
                                                                                          ::database::type::RowId date_id,
                                                                                          ::database::type::RowId user_id,
                                                                                          ::database::type::RowId command_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::CountCommandsForUserDailyStatisticFromLogs: Function call";

  return raw_database_functions_->CountCommandsForUserDailyStatisticFromLogs(agent_name_id, date_id, user_id, command_id);
}

void DatabaseFunctions::AddDailySystemStatistic(const detail::entity::DailySystemStatistic &statistics) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::AddDailySystemStatistic: Function call";

  raw_database_functions_->AddDailySystemStatistic(statistics);
}

::database::type::RowIds DatabaseFunctions::GetDateIdsWithoutCreatedDailySystemStatistic(::database::type::RowId agent_name_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetDateIdsWithoutCreatedDailySystemStatistic: Function call";

  return raw_database_functions_->GetDateIdsWithoutCreatedDailySystemStatistic(agent_name_id);
}

::database::type::RowIds DatabaseFunctions::GetAgentIdsWithoutConfiguration() {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetAgentIdsWithoutConfiguration: Function call";

  return raw_database_functions_->GetAgentIdsWithoutConfiguration();
}

::database::type::RowIds DatabaseFunctions::GetAgentsIdsWithConfiguration() {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetAgentsIdsWithConfigurations: Function call";

  return raw_database_functions_->GetAgentsIdsWithConfiguration();
}

void DatabaseFunctions::AddDailyUserStatistic(const ::bash::database::detail::entity::DailyUserStatistic &us) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::AddDailyUserStatistic: Function call";

  raw_database_functions_->AddDailyUserStatistic(us);
}

::database::type::RowId DatabaseFunctions::GetDailyUserStatisticId(::database::type::RowId agent_name_id,
                                                                   ::database::type::RowId user_id,
                                                                   ::database::type::RowId date_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetDailyUserStatisticId: Function call";

  return raw_database_functions_->GetDailyUserStatisticId(agent_name_id, user_id, date_id);
}

void DatabaseFunctions::AddDailyUserCommandStatistic(const ::bash::database::detail::entity::DailyUserCommandStatistic &ucs) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetDailyUserStatisticId: Function call";

  return raw_database_functions_->AddDailyUserCommandStatistic(ucs);
}

type::AnomalyDetectionConfigurations DatabaseFunctions::GetAnomalyDetectionConfigurations() {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetAnomalyDetectionConfigurations: Function call";

  auto raw_configs = raw_database_functions_->GetAnomalyDetectionConfigurations();

  type::AnomalyDetectionConfigurations configs;
  type::AnomalyDetectionConfiguration c;

  for (const auto &config : raw_configs) {
    c.id = config.id;
    c.agent_name_id = config.agent_name_id;
    c.begin_date = general_database_functions_->GetDateById(config.begin_date_id);
    c.end_date = general_database_functions_->GetDateById(config.end_date_id);

    configs.push_back(c);
  }

  return configs;
}

void DatabaseFunctions::RemoveAnomalyDetectionConfiguration(::database::type::RowId id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::RemoveAnomalyDetectionConfiguration: Function call";

  raw_database_functions_->RemoveAnomalyDetectionConfiguration(id);
}

void DatabaseFunctions::AddAnomalyDetectionConfiguration(const type::AnomalyDetectionConfiguration &configuration) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::AddAnomalyDetectionConfiguration: Function call";

  detail::entity::AnomalyDetectionConfiguration c;
  c.agent_name_id = configuration.agent_name_id;
  c.begin_date_id = general_database_functions_->GetDateId(configuration.begin_date);
  c.end_date_id = general_database_functions_->GetDateId(configuration.end_date);

  raw_database_functions_->AddAnomalyDetectionConfiguration(c);
}

::database::type::RowId DatabaseFunctions::GetConfigurationIdForAgent(::database::type::RowId agent_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetConfigurationIdForAgent: Function call";

  return raw_database_functions_->GetConfigurationIdForAgent(agent_id);
}

void DatabaseFunctions::RemoveAllCommandsFromConfiguration(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::RemoveAllCommandsFromConfiguration: Function call";

  raw_database_functions_->RemoveAllCommandsFromConfiguration(configuration_id);
}

void DatabaseFunctions::AddDefaultCommandsToConfiguration(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::AddDefaultCommandsToConfiguration: Function call";

  raw_database_functions_->AddDefaultCommandsToConfiguration(configuration_id);
}

void DatabaseFunctions::AddCommandStatistic(const detail::entity::CommandStatistic &statistic) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::AddCommandStatistic: Function call";

  raw_database_functions_->AddCommandStatistic(statistic);
}

bool DatabaseFunctions::IsCommandStatisticExist(::database::type::RowId agent_name_id,
                                                ::database::type::RowId command_id,
                                                ::database::type::RowId begin_date_id,
                                                ::database::type::RowId end_date_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::IsCommandStatisticExist: Function call";

  return raw_database_functions_->IsCommandStatisticExist(agent_name_id, command_id, begin_date_id, end_date_id);
}

detail::entity::CommandsStatistics DatabaseFunctions::GetCommandsStatistics(::database::type::RowId agent_name_id,
                                                                            ::database::type::RowId begin_date_id,
                                                                            ::database::type::RowId end_date_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetCommandStatistic: Function call";

  return raw_database_functions_->GetCommandsStatistics(agent_name_id, begin_date_id, end_date_id);
}

detail::entity::CommandsStatistics DatabaseFunctions::GetCommandsStatistics(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetCommandStatistic: Function call";

  return raw_database_functions_->GetCommandsStatistics(configuration_id);
}

::database::type::RowIds DatabaseFunctions::GetMarkedCommandsIds(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetMarkedCommandsIds: Function call";

  return raw_database_functions_->GetMarkedCommandsIds(configuration_id);
}

::database::type::RowsCount DatabaseFunctions::CommandSummary(::database::type::RowId command_id,
                                                              ::database::type::RowIds date_range_ids) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::CommandSummary: Function call";

  return raw_database_functions_->CommandSummary(command_id, date_range_ids);
}

void DatabaseFunctions::AddSelectedCommandsIds(::database::type::RowId configuration_id,
                                               ::database::type::RowIds command_names_ids) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::AddSelectedCommandsIds: Function call";

  raw_database_functions_->AddSelectedCommandsIds(configuration_id, command_names_ids);
}

::database::type::RowIds DatabaseFunctions::GetNotCalculatedDatesIdsFromLogs(::database::type::RowId agent_name_id,
                                                                             ::database::type::RowId user_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetNotCalculatedDatesIdsFromLogs: Function call";

  return raw_database_functions_->GetNotCalculatedDatesIdsFromLogs(agent_name_id, user_id);
}

::database::type::RowIds DatabaseFunctions::GetCommandsIdsFromLogs(::database::type::RowId agent_name_id,
                                                                   ::database::type::RowId user_id,
                                                                   ::database::type::RowId date_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetCommandsIdsFromLogs: Function call";

  return raw_database_functions_->GetCommandsIdsFromLogs(agent_name_id, user_id, date_id);
}

void DatabaseFunctions::AddDailyUserStatisticsToConfiguration(::database::type::RowId configuration_id,
                                                              const ::database::type::RowIds &date_range_ids) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::AddDailyUserStatisticsToConfiguration: Function call";

  raw_database_functions_->AddDailyUserStatisticsToConfiguration(configuration_id, date_range_ids);
}

void DatabaseFunctions::RemoveDailyStatisticsFromConfiguration(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::RemoveDailyStatisticsFromConfiguration: Function call";

  raw_database_functions_->RemoveDailyStatisticsFromConfiguration(configuration_id);
}

::bash::database::detail::entity::DailyUserStatistics DatabaseFunctions::GetDailyUserStatisticsFromConfiguration(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::database::DatabaseFunctions::GetDailyUserStatisticsFromConfiguration: Function call";

  return raw_database_functions_->GetDailyUserStatisticsFromConfiguration(configuration_id);
}

DatabaseFunctions::DatabaseFunctions(::bash::database::detail::RawDatabaseFunctionsInterfacePtr raw_database_functions,
                                     ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) :
raw_database_functions_(raw_database_functions),
general_database_functions_(general_database_functions) {
}

}

}
