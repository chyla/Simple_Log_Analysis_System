/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "classificator.h"

#include <slas/util/run_partially.h>

#include <algorithm>
#include <vector>
#include <fstream>
#include <boost/log/trivial.hpp>

#include "src/bash/analyzer/detail/command_summary_divider/command_summary_divider.h"
#include "src/library/fann/fann_wrapper.h"
#include "src/library/fann/fann_guard.h"

namespace bash
{

namespace analyzer
{

namespace detail
{

namespace classificator
{

ClassificatorPtr Classificator::Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                       ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                       const std::string &neural_network_data_directory) {
  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Create: Function call";

  auto fann_wrapper = ::library::fann::FannWrapper::Create();

  return Create(database_functions, general_database_functions, neural_network_data_directory, fann_wrapper);
}

ClassificatorPtr Classificator::Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                       ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                       const std::string &neural_network_data_directory,
                                       ::library::fann::detail::FannWrapperInterfacePtr fann_wrapper) {
  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Create: Function call";

  return ClassificatorPtr(new Classificator(database_functions, general_database_functions, neural_network_data_directory, fann_wrapper));
}

void Classificator::Analyze() {
  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Analyze: Function call";

  constexpr int MAX_ROWS_IN_MEMORY = 100;

  command_summary_divider::CommandSummaryDivider divider;
  unsigned selected_commands_position = 0;
  unsigned commands_statistics_position = 0;
  fann_type *calc_out;
  fann_type input[100];

  auto configurations = database_functions_->GetAnomalyDetectionConfigurations();
  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Analyze: Found " << configurations.size() << " configurations";

  for (const auto &c : configurations) {
    BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Analyze: Create neural network";
    struct fann *ann = fann_wrapper_->CreateFromFile(neural_network_data_directory_ + "/network-" + std::to_string(c.id) + ".data");
    ::library::fann::FannGuard fann_guard(ann);

    auto selected_commands_ids = database_functions_->GetMarkedCommandsIds(c.id);
      BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Analyze: Found " << selected_commands_ids.size() << " selected commands ids";

    for (const auto &id : selected_commands_ids)
      BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Analyze: id: " << id;

    auto users = database_functions_->GetUsersIdsFromSelectedDailyStatisticsInConfiguration(c.id);

    auto daily_user_statistics_count = database_functions_->CountDailyUserStatisticsForAgentWithClassification(c.agent_name_id, ::database::type::Classification::UNKNOWN);

    util::RunPartially(MAX_ROWS_IN_MEMORY, daily_user_statistics_count, [&](long long part_count, long long offset) {
      auto daily_user_statistics = database_functions_->GetDailyUserStatisticsForAgentWithClassification(c.agent_name_id, ::database::type::Classification::UNKNOWN, part_count, 0);

      for (const auto &statistic : daily_user_statistics) {
        std::fill(input, input + 100, 0);

        auto commands_statistics = database_functions_->GetSelectedDailyUserCommandsStatistics(statistic.id);
        BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Analyze: Found " << commands_statistics.size() << " commands statistics";

        selected_commands_position = 0;
        commands_statistics_position = 0;
        while (selected_commands_position < selected_commands_ids.size()
            && commands_statistics_position < commands_statistics.size()) {
          const auto &command_statistic = commands_statistics.at(commands_statistics_position);
          BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Analyze: Position " << selected_commands_position;

          if (command_statistic.command_id == selected_commands_ids.at(selected_commands_position)) {
            BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Analyze: Found command with id " << command_statistic.command_id;

            input[selected_commands_position] = command_statistic.summary;
            commands_statistics_position++;
          }

          selected_commands_position++;
        }

        std::transform(input, input + 100, input, divider);

        for (int i = 0; i < 100; i++) {
          BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Analyze: Input value " << i << " : " << input[i];
        }

        calc_out = fann_wrapper_->Run(ann, input);

        ::database::type::RowIds::size_type user_position = 0;
        fann_type output_value = 0;
        for (::database::type::RowIds::size_type i = 0; i < users.size(); i++) {
          BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Analyze: Output value " << i << " : " << calc_out[i];

          if (calc_out[i] >= output_value) {
            output_value = calc_out[i];
            user_position = i;
          }
        }

        BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Analyze: Found max output value for user_id " << users.at(user_position) << " at position " << user_position << ": " << calc_out[user_position];

        constexpr double anomaly_threshold = 0.5;
        if (output_value >= anomaly_threshold && users.at(user_position) == statistic.user_id) {
          BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Analyze: Marking statistic with id " << statistic.id << " as normal";
          database_functions_->SetDailyUserStatisticsClassification({statistic.id}, ::database::type::Classification::NORMAL);
        }
        else {
          BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::classificator::Classificator::Analyze: Marking statistic with id " << statistic.id << " as anomaly";
          database_functions_->SetDailyUserStatisticsClassification({statistic.id}, ::database::type::Classification::ANOMALY);
        }
      }
    });
  }
}

Classificator::Classificator(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                             ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                             const std::string &neural_network_data_directory,
                             ::library::fann::detail::FannWrapperInterfacePtr fann_wrapper) :
database_functions_(database_functions),
general_database_functions_(general_database_functions),
neural_network_data_directory_(neural_network_data_directory),
fann_wrapper_(fann_wrapper) {
}

}

}

}

}
