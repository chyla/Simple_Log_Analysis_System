/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "scripts.h"
#include "src/bash/database/detail/entity/anomaly_detection_configuration.h"
#include "src/bash/database/detail/entity/system_user.h"

#include <boost/log/trivial.hpp>

namespace bash
{

namespace domain
{

ScriptsPtr Scripts::Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                           ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) {
  return ScriptsPtr(new Scripts(database_functions, general_database_functions));
}

void Scripts::AddLog(const ::type::BashLogEntry &log_entry) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::AddLog: Function call";

  general_database_functions_->AddDate(log_entry.utc_time.GetDate());
  general_database_functions_->AddTime(log_entry.utc_time.GetTime());
  general_database_functions_->AddAgentName(log_entry.agent_name);
  database_functions_->AddSystemUser(log_entry.user_id);
  database_functions_->AddCommand(log_entry.command);

  database_functions_->AddLog(log_entry);
}

void Scripts::CreateDailySystemStatistics() {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::CreateDailySystemStatistics: Function call";

  ::bash::database::detail::entity::DailySystemStatistic stat;

  auto agents_names_ids = general_database_functions_->GetAgentsIds();
  auto commands_ids = database_functions_->GetAllCommandsIds();

  for (auto agent_name_id : agents_names_ids) {
    auto dates_ids = database_functions_->GetDateIdsWithoutCreatedDailySystemStatistic(agent_name_id);

    for (auto date_id : dates_ids) {
      for (auto command_id : commands_ids) {
        auto count = database_functions_->CountCommandsForDailySystemStatistic(agent_name_id, date_id, command_id);

        stat.agent_name_id = agent_name_id;
        stat.date_id = date_id;
        stat.command_id = command_id;
        stat.summary = count;

        database_functions_->AddDailySystemStatistic(stat);
      }
    }
  }
}

::bash::domain::type::UnconfiguredAgents Scripts::GetUnconfigurentAgents() {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::GetUnconfigurentAgents: Function call";

  auto ids = database_functions_->GetAgentIdsWithoutConfiguration();

  ::bash::domain::type::UnconfiguredAgents agents;
  agents.reserve(ids.size());

  for (const auto &id : ids) {
    auto name = general_database_functions_->GetAgentNameById(id);

    agents.push_back({id, name});
  }

  return agents;
}

::bash::domain::type::AnomalyDetectionConfigurations Scripts::GetAnomalyDetectionConfigurations() {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::GetAnomalyDetectionConfigurations: Function call";

  auto raw_configurations = database_functions_->GetAnomalyDetectionConfigurations();

  ::bash::domain::type::AnomalyDetectionConfigurations configurations;
  configurations.reserve(raw_configurations.size());

  ::bash::domain::type::AnomalyDetectionConfiguration c;
  for (const auto &config : raw_configurations) {
    c.id = config.id;
    c.agent_name = general_database_functions_->GetAgentNameById(config.agent_name_id);
    c.agent_name_id = config.agent_name_id;
    c.begin_date = config.begin_date;
    c.end_date = config.end_date;

    configurations.push_back(c);
  }

  return configurations;
}

void Scripts::RemoveAnomalyDetectionConfiguration(::database::type::RowId id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::RemoveAnomalyDetectionConfiguration: Function call";

  database_functions_->RemoveAnomalyDetectionConfiguration(id);
}

void Scripts::SaveConfiguration(::database::type::RowId agent_name_id,
                                const ::type::Date &begin_date,
                                const ::type::Date &end_date) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::SaveConfiguration: Function call";

  ::bash::database::type::AnomalyDetectionConfiguration c;
  c.agent_name_id = agent_name_id;
  c.begin_date = begin_date;
  c.end_date = end_date;
  c.changed = true;

  database_functions_->AddAnomalyDetectionConfiguration(c);
}

::database::type::RowId Scripts::GetConfigurationIdForAgent(::database::type::RowId agent_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::GetConfigurationIdForAgent: Function call";

  return database_functions_->GetConfigurationIdForAgent(agent_id);
}

void Scripts::AddDefaultCommandsToAgentConfiguration(::database::type::RowId agent_name_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::AddDefaultCommandsToAgentConfiguration: Function call";

  auto configuration_id = database_functions_->GetConfigurationIdForAgent(agent_name_id);

  database_functions_->AddDefaultCommandsToConfiguration(configuration_id);
}

void Scripts::RemoveAllCommandsFromAgentConfiguration(::database::type::RowId agent_name_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::RemoveAllCommandsFromAgentConfiguration: Function call";

  auto configuration_id = database_functions_->GetConfigurationIdForAgent(agent_name_id);

  database_functions_->RemoveAllCommandsFromConfiguration(configuration_id);
}

void Scripts::RemoveAllCommandsFromConfiguration(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::RemoveAllCommandsFromConfiguration: Function call";

  database_functions_->RemoveAllCommandsFromConfiguration(configuration_id);
}

void Scripts::AddDailyUserStatisticsToConfiguration(::database::type::RowId agent_name_id,
                                                    const ::type::Date &begin_date,
                                                    const ::type::Date &end_date) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::AddDailyUserStatisticsToConfiguration: Function call";

  auto id = database_functions_->GetConfigurationIdForAgent(agent_name_id);
  auto date_range_ids = general_database_functions_->GetDateRangeIds(begin_date, end_date);

  database_functions_->AddDailyUserStatisticsToConfiguration(id, date_range_ids);
}

void Scripts::RemoveDailyStatisticsFromConfiguration(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::RemoveDailyStatisticsFromConfiguration: Function call";

  database_functions_->RemoveDailyStatisticsFromConfiguration(configuration_id);
}

::bash::domain::type::DailyUserStatistics Scripts::GetDailyUserStatisticsFromConfiguration(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::GetDailyUserStatisticsFromConfiguration: Function call";

  auto raw_statistics = database_functions_->GetDailyUserStatisticsFromConfiguration(configuration_id);

  ::bash::domain::type::DailyUserStatistics statistics;
  statistics.reserve(raw_statistics.size());

  ::bash::domain::type::DailyUserStatistic s;
  for (const auto &r : raw_statistics) {
    auto system_user = database_functions_->GetSystemUserById(r.user_id);

    s.agent_name = general_database_functions_->GetAgentNameById(r.agent_name_id);
    s.classification = r.classification;
    s.date = general_database_functions_->GetDateById(r.date_id);
    s.uid = system_user.uid;
    s.id = r.id;

    statistics.push_back(s);
  }

  return statistics;
}

::bash::domain::type::DailyUserStatistics Scripts::GetDailyUserStatisticsForAgent(::database::type::RowId agent_name_id,
                                                                                  const ::type::Date &begin_date,
                                                                                  const ::type::Date &end_date) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::GetDailyUserStatisticsForAgent: Function call";

  auto date_range_ids = general_database_functions_->GetDateRangeIds(begin_date, end_date);

  auto raw_statistics = database_functions_->GetDailyUserStatisticsForAgent(agent_name_id, date_range_ids);

  ::bash::domain::type::DailyUserStatistics statistics;
  statistics.reserve(raw_statistics.size());

  ::bash::domain::type::DailyUserStatistic s;
  for (const auto &r : raw_statistics) {
    auto system_user = database_functions_->GetSystemUserById(r.user_id);

    s.agent_name = general_database_functions_->GetAgentNameById(r.agent_name_id);
    s.classification = r.classification;
    s.date = general_database_functions_->GetDateById(r.date_id);
    s.uid = system_user.uid;
    s.id = r.id;

    statistics.push_back(s);
  }

  return statistics;
}

::database::entity::AgentNames Scripts::GetAgentsWithExistingDailyUserStatistics() {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::GetAgentsWithExistingDailyUserStatistics: Function call";

  return database_functions_->GetAgentsWithExistingDailyUserStatistics();
}

void Scripts::UpdateDailyUserStatisticsClassification(::database::type::RowId configuration_id,
                                                      ::database::type::RowIds normal_ids,
                                                      ::database::type::RowIds anomaly_ids) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::UpdateDailyUserStatisticsClassification: Function call";

  database_functions_->SetDailyUserStatisticsClassification(normal_ids, ::database::type::Classification::NORMAL);
  database_functions_->SetDailyUserStatisticsClassification(anomaly_ids, ::database::type::Classification::ANOMALY);

  database_functions_->MarkConfigurationAsChanged(configuration_id);
}

void Scripts::CalculateCommandStatistics(::database::type::RowId agent_name_id,
                                         const ::type::Date &begin_date,
                                         const ::type::Date &end_date) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::CalculateCommandStatistics: Function call";

  auto begin_date_id = general_database_functions_->AddAndGetDateId(begin_date);
  auto end_date_id = general_database_functions_->AddAndGetDateId(end_date);
  auto date_range_ids = general_database_functions_->GetDateRangeIds(begin_date, end_date);

  auto all_commands_ids = database_functions_->GetAllCommandsIds();

  ::bash::database::detail::entity::CommandStatistic cs;
  cs.agent_name_id = agent_name_id;
  cs.begin_date_id = begin_date_id;
  cs.end_date_id = end_date_id;

  bool statistics_exists;
  for (auto command_id : all_commands_ids) {
    statistics_exists = database_functions_->IsCommandStatisticExist(agent_name_id, command_id, begin_date_id, end_date_id);

    if (statistics_exists == false) {
      BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::CalculateCommandStatistics: Statistics not exists for "
          << "agent_name_id=" << agent_name_id << "; command_id=" << command_id
          << "; begin_date_id=" << begin_date_id << "; end_date_id=" << end_date_id;

      auto summary = database_functions_->CommandSummary(command_id, date_range_ids);

      cs.command_id = command_id;
      cs.summary = summary;

      database_functions_->AddCommandStatistic(cs);
    }
  }

  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::CalculateCommandStatistics: Statistics calculated";
}

::bash::domain::type::CommandsStatistics Scripts::GetCommandsStatistics(::database::type::RowId agent_name_id,
                                                                        const ::type::Date &begin_date,
                                                                        const ::type::Date &end_date) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::GetCommandsStatistics: Function call";

  auto begin_date_id = general_database_functions_->AddAndGetDateId(begin_date);
  auto end_date_id = general_database_functions_->AddAndGetDateId(end_date);

  auto raw_statistics = database_functions_->GetCommandsStatistics(agent_name_id, begin_date_id, end_date_id);

  ::bash::domain::type::CommandsStatistics statistics;
  statistics.reserve(raw_statistics.size());

  ::bash::domain::type::CommandStatistic s;
  for (const auto &st : raw_statistics) {
    s.command_name_id = st.command_id;
    s.summary_count = st.summary;
    s.command_name = database_functions_->GetCommandNameById(st.command_id);

    statistics.push_back(s);
  }

  return statistics;
}

::bash::domain::type::CommandsStatistics Scripts::GetCommandsStatistics(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::GetCommandsStatistics: Function call";

  auto raw_statistics = database_functions_->GetCommandsStatistics(configuration_id);

  ::bash::domain::type::CommandsStatistics statistics;
  statistics.reserve(raw_statistics.size());

  ::bash::domain::type::CommandStatistic s;
  for (const auto &st : raw_statistics) {
    s.command_name_id = st.command_id;
    s.summary_count = st.summary;
    s.command_name = database_functions_->GetCommandNameById(st.command_id);

    statistics.push_back(s);
  }

  return statistics;
}

::database::type::RowIds Scripts::GetMarkedCommandsIds(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::GetMarkedCommandsIds: Function call";

  return database_functions_->GetMarkedCommandsIds(configuration_id);
}

void Scripts::SaveSelectedCommands(::database::type::RowId configuration_id, ::database::type::RowIds command_names_ids) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::SaveSelectedCommands: Function call";

  database_functions_->RemoveAllCommandsFromConfiguration(configuration_id);

  database_functions_->AddSelectedCommandsIds(configuration_id, command_names_ids);

  database_functions_->MarkConfigurationAsChanged(configuration_id);
}

void Scripts::SelectDefaultCommands(::database::type::RowId configuration_id) {
  BOOST_LOG_TRIVIAL(debug) << "bash::domain::Scripts::SelectDefaultCommands: Function call";

  database_functions_->RemoveAllCommandsFromConfiguration(configuration_id);

  database_functions_->AddDefaultCommandsToConfiguration(configuration_id);

  database_functions_->MarkConfigurationAsChanged(configuration_id);
}

Scripts::Scripts(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                 ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) :
database_functions_(database_functions),
general_database_functions_(general_database_functions) {
}

}

}
