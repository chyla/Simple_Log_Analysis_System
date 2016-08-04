/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "daily_user_statistics_creator_interface.h"

#include "src/bash/database/detail/database_functions_interface.h"
#include "src/database/detail/general_database_functions_interface.h"

namespace bash
{

namespace analyzer
{

namespace detail
{

class DailyUserStatisticsCreator;
typedef std::shared_ptr<DailyUserStatisticsCreator> DailyUserStatisticsCreatorPtr;

class DailyUserStatisticsCreator : public DailyUserStatisticsCreatorInterface {
 public:
  virtual ~DailyUserStatisticsCreator() = default;

  static DailyUserStatisticsCreatorPtr Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                              ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);

  void CreateStatistics(const ::type::Date &today) override;

 private:
  DailyUserStatisticsCreator(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                             ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);

  ::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions_;
  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;
};

}

}

}
