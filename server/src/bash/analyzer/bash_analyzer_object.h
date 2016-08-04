/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "src/analyzer/analyzer_object_interface.h"
#include "src/bash/domain/detail/scripts_interface.h"
#include "src/bash/analyzer/detail/daily_user_statistics_creator_interface.h"
#include "src/bash/database/detail/database_functions_interface.h"
#include "src/database/detail/general_database_functions_interface.h"
#include "detail/system_interface.h"
#include "detail/network_trainer/network_trainer_interface.h"
#include "detail/classificator/classificator_interface.h"

#include <patlms/type/date.h>

#include <memory>

namespace bash
{

namespace analyzer
{

class BashAnalyzerObject;
typedef std::shared_ptr<BashAnalyzerObject> BashAnalyzerObjectPtr;

class BashAnalyzerObject : public ::analyzer::AnalyzerObjectInterface {
 public:
  virtual ~BashAnalyzerObject() = default;

  static BashAnalyzerObjectPtr Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                      ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                      ::bash::domain::detail::ScriptsInterfacePtr scripts_interface,
                                      const std::string &neural_network_data_directory);

  static BashAnalyzerObjectPtr Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                      ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                      ::bash::domain::detail::ScriptsInterfacePtr scripts_interface,
                                      detail::SystemInterfacePtr system_interface,
                                      const std::string &neural_network_data_directory);

  void Analyze() override;

 private:
  BashAnalyzerObject(detail::DailyUserStatisticsCreatorInterfacePtr daily_user_statistics_creator,
                     detail::network_trainer::NetworkTrainerInterfacePtr network_trainer,
                     detail::classificator::ClassificatorInterfacePtr classificator,
                     ::bash::domain::detail::ScriptsInterfacePtr scripts_interface,
                     detail::SystemInterfacePtr system_interface);

  ::type::Date GetCurrentDate() const;

  detail::DailyUserStatisticsCreatorInterfacePtr daily_user_statistics_creator_;
  detail::network_trainer::NetworkTrainerInterfacePtr network_trainer_;
  detail::classificator::ClassificatorInterfacePtr classificator_;
  ::bash::domain::detail::ScriptsInterfacePtr scripts_interface_;
  detail::SystemInterfacePtr system_interface_;
};

}

}
