/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef BASH_ANALYZER_OBJECT_H
#define BASH_ANALYZER_OBJECT_H

#include "src/analyzer/analyzer_object_interface.h"
#include "src/bash/domain/detail/scripts_interface.h"
#include "src/bash/analyzer/detail/daily_user_statistics_creator_interface.h"
#include "src/bash/database/detail/database_functions_interface.h"
#include "src/database/detail/general_database_functions_interface.h"
#include "detail/system_interface.h"

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
                                      ::bash::domain::detail::ScriptsInterfacePtr scripts_interface);

  static BashAnalyzerObjectPtr Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                      ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                      ::bash::domain::detail::ScriptsInterfacePtr scripts_interface,
                                      detail::SystemInterfacePtr system_interface);

  void Analyze() override;

 private:
  BashAnalyzerObject(detail::DailyUserStatisticsCreatorInterfacePtr daily_user_statistics_creator,
                     ::bash::domain::detail::ScriptsInterfacePtr scripts_interface,
                     detail::SystemInterfacePtr system_interface);

  ::type::Date GetCurrentDate() const;

  detail::DailyUserStatisticsCreatorInterfacePtr daily_user_statistics_creator_;
  ::bash::domain::detail::ScriptsInterfacePtr scripts_interface_;
  detail::SystemInterfacePtr system_interface_;
};

}

}

#endif /* BASH_ANALYZER_OBJECT_H */
