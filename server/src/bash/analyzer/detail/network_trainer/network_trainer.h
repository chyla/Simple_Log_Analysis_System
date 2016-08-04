/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "network_trainer_interface.h"

#include "src/bash/database/detail/database_functions_interface.h"
#include "src/database/detail/general_database_functions_interface.h"
#include "src/library/fann/detail/fann_wrapper_interface.h"

namespace bash
{

namespace analyzer
{

namespace detail
{

namespace network_trainer
{

class NetworkTrainer;
typedef std::shared_ptr<NetworkTrainer> NetworkTrainerPtr;

class NetworkTrainer : public NetworkTrainerInterface {
 public:
  virtual ~NetworkTrainer() = default;

  static NetworkTrainerPtr Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                  const std::string &neural_network_data_directory);

  static NetworkTrainerPtr Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                  const std::string &neural_network_data_directory,
                                  ::library::fann::detail::FannWrapperInterfacePtr fann_wrapper);

  void Train() override;

 private:
  ::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions_;
  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;
  const std::string neural_network_data_directory_;
  ::library::fann::detail::FannWrapperInterfacePtr fann_wrapper_;

  NetworkTrainer(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                 ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                 const std::string &neural_network_data_directory,
                 ::library::fann::detail::FannWrapperInterfacePtr fann_wrapper);

  void CreateLearningSetFile(const ::bash::database::type::AnomalyDetectionConfiguration &configuration);
  void CreateNetworkConfiguration(const ::bash::database::type::AnomalyDetectionConfiguration &configuration);
};

}

}

}

}
