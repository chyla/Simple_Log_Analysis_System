/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "daily_user_statistics_creator.h"
#include "src/bash/database/detail/entity/daily_user_statistic.h"

#include <boost/log/trivial.hpp>

namespace bash
{

namespace analyzer
{

namespace detail
{

DailyUserStatisticsCreatorPtr DailyUserStatisticsCreator::Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                                                 ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) {
  return DailyUserStatisticsCreatorPtr(new DailyUserStatisticsCreator(database_functions, general_database_functions));
}

void DailyUserStatisticsCreator::CreateStatistics(const ::type::Date &today) {
  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::DailyUserStatisticsCreator::CreateStatistics: Function call";

  ::bash::database::detail::entity::DailyUserStatistic us;

  auto agents = database_functions_->GetAgentsIdsWithConfiguration();
  for (const auto &agent : agents) {
    us.agent_name_id = agent;

    auto users = database_functions_->GetSystemUsersIdsFromLogs(agent);
    for (const auto &user : users) {
      us.user_id = user;

      auto dates = database_functions_->GetNotCalculatedDatesIdsFromLogs(agent, user);
      for (const auto &date : dates) {
        us.date_id = date;

        auto commands = database_functions_->GetCommandsIdsFromLogs(agent, user, date);
        for (const auto &command : commands) {
          us.command_id = command;
          us.classification = ::database::type::Classification::UNKNOWN;
          us.summary = database_functions_->CountCommandsForUserDailyStatisticFromLogs(agent, user, date, command);

          database_functions_->AddDailyUserStatistic(us);
        }
      }
    }
  }
}

DailyUserStatisticsCreator::DailyUserStatisticsCreator(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                                       ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions) :
database_functions_(database_functions),
general_database_functions_(general_database_functions) {
}

}

}

}
