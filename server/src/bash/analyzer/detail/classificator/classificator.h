/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "classificator_interface.h"

#include "src/bash/database/detail/database_functions_interface.h"
#include "src/database/detail/general_database_functions_interface.h"
#include "src/library/fann/detail/fann_wrapper_interface.h"

namespace bash
{

namespace analyzer
{

namespace detail
{

namespace classificator
{

class Classificator;
typedef std::shared_ptr<Classificator> ClassificatorPtr;

class Classificator : public ClassificatorInterface {
 public:
  virtual ~Classificator() = default;

  static ClassificatorPtr Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                  const std::string &neural_network_data_directory);

  static ClassificatorPtr Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                  const std::string &neural_network_data_directory,
                                  ::library::fann::detail::FannWrapperInterfacePtr fann_wrapper);

  void Analyze() override;

 private:
  ::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions_;
  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;
  const std::string neural_network_data_directory_;
  ::library::fann::detail::FannWrapperInterfacePtr fann_wrapper_;

  Classificator(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                 ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                 const std::string &neural_network_data_directory,
                 ::library::fann::detail::FannWrapperInterfacePtr fann_wrapper);
};

}

}

}

}
