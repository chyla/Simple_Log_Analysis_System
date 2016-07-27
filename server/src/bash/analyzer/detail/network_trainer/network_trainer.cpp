/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "network_trainer.h"

#include <patlms/util/run_partially.h>

#include <algorithm>
#include <vector>
#include <fstream>
#include <boost/log/trivial.hpp>

namespace bash
{

namespace analyzer
{

namespace detail
{

namespace network_trainer
{

NetworkTrainerPtr NetworkTrainer::Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                         ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                         const std::string &neural_network_data_directory) {
  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::Create: Function call";

  return NetworkTrainerPtr(new NetworkTrainer(database_functions, general_database_functions, neural_network_data_directory));
}

void NetworkTrainer::Train() {
  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::Train: Function call";

  auto configurations = database_functions_->GetAnomalyDetectionConfigurations();

  for (const auto &c : configurations) {
    BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::Train: Is configuration changed for agent " << c.agent_name_id << "?: " << c.changed;

    if (c.changed) {
      CreateLearningSetFile(c);
    }
  }
}

NetworkTrainer::NetworkTrainer(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                               ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                               const std::string &neural_network_data_directory) :
database_functions_(database_functions),
general_database_functions_(general_database_functions),
neural_network_data_directory_(neural_network_data_directory) {
}

void NetworkTrainer::CreateLearningSetFile(const ::bash::database::type::AnomalyDetectionConfiguration &configuration) {
  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Function call";

  auto users = database_functions_->GetUsersIdsFromSelectedDailyStatisticsInConfiguration(configuration.id);

  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Found " << users.size() << " users";

  constexpr::database::type::RowsCount MAX_ROWS_IN_MEMORY = 100;
  int number_of_inputs = 100;
  int number_of_outputs = users.size();
  long long learning_set_size = database_functions_->GetNumberOfSelectedDailyStatisticsInConfiguration(configuration.id);
  std::string file_path = neural_network_data_directory_ + "/training-" + std::to_string(configuration.id) + ".data";

  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Number of inputs: " << number_of_inputs;
  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Number of outputs: " << number_of_outputs;
  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Learning set size: " << learning_set_size;
  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Training file path: " << file_path;

  std::vector<double> input;
  input.resize(number_of_inputs, 0);

  std::vector<int> output;
  output.resize(number_of_outputs, -1);

  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Opening training file";
  std::fstream file(file_path.c_str(), std::ios::out | std::ios::trunc);

  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Writing training data";
  file << learning_set_size << ' ' << number_of_inputs << ' ' << number_of_outputs << '\n';

  auto selected_commands_ids = database_functions_->GetMarkedCommandsIds(configuration.id);
  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Found " << selected_commands_ids.size() << " selected commands ids";

  for (const auto &id : selected_commands_ids)
    BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: id: " << id;

  unsigned selected_commands_position = 0;
  unsigned commands_statistics_position = 0;
  int user_output_position = 0;
  for (const auto &user_id : users) {
    BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Writing data for user id (from database) " << user_id;

    auto daily_user_statistics_count = database_functions_->GetSelectedDailyUserStatisticsCountFromConfigurationByUser(configuration.id, user_id);
    BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Found " << daily_user_statistics_count << " daily user statistics";

    util::RunPartially(MAX_ROWS_IN_MEMORY, daily_user_statistics_count, [&](long long part_count, long long offset) {
      auto daily_user_statistics = database_functions_->GetSelectedDailyUserStatisticsFromConfigurationByUser(configuration.id, user_id, part_count, offset);
       BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Found " << daily_user_statistics.size() << " statistics in part";

      for (const auto &statistic : daily_user_statistics) {
        if (statistic.classification != ::database::type::Classification::UNKNOWN) {
          std::fill(input.begin(), input.end(), 0);

          auto commands_statistics = database_functions_->GetSelectedDailyUserCommandsStatistics(statistic.id);
          BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Found " << commands_statistics.size() << " selected daily user commands statistics with statistic id " << statistic.id;

          selected_commands_position = 0;
          commands_statistics_position = 0;
          while (selected_commands_position < selected_commands_ids.size()
              && commands_statistics_position < commands_statistics.size()) {
            const auto &command_statistic = commands_statistics.at(commands_statistics_position);
            BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Position " << selected_commands_position;

            if (command_statistic.command_id == selected_commands_ids.at(selected_commands_position)) {
              BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Found command with id " << command_statistic.command_id;

              input.at(selected_commands_position) = command_statistic.summary;
              commands_statistics_position++;
            }

            selected_commands_position++;
          }

          if (statistic.classification == ::database::type::Classification::ANOMALY)
            output.at(user_output_position) = -1;
          else if (statistic.classification == ::database::type::Classification::NORMAL)
            output.at(user_output_position) = 1;

          BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Classification set: " << output.at(user_output_position);

          BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Saving to file";

          for (const auto &v : input)
            file << v << " ";
          file << '\n';

          for (const auto &v : output)
            file << v << " ";
          file << '\n';
        }
        else {
          BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Classification of session statistics is UNKNOWN (statistic id = " << statistic.id << ")";
        }
      }
    });

    output.at(user_output_position) = -1;
    user_output_position++;
  }

  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Closing training file";
  file.close();

  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::detail::network_trainer::NetworkTrainer::CreateLearningSetFile: Done";
}

}

}

}

}
