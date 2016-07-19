/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "bash_analyzer_object.h"

#include "detail/daily_user_statistics_creator.h"
#include "detail/system.h"

#include <boost/log/trivial.hpp>

namespace bash
{

namespace analyzer
{

BashAnalyzerObjectPtr BashAnalyzerObject::Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                                 ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                                 ::bash::domain::detail::ScriptsInterfacePtr scripts_interface) {
  auto dusc = detail::DailyUserStatisticsCreator::Create(database_functions, general_database_functions);
  auto system = detail::System::Create();

  return BashAnalyzerObjectPtr(new BashAnalyzerObject(dusc, scripts_interface, system));
}

BashAnalyzerObjectPtr BashAnalyzerObject::Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                                                 ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                                 ::bash::domain::detail::ScriptsInterfacePtr scripts_interface,
                                                 detail::SystemInterfacePtr system_interface) {
  auto dusc = detail::DailyUserStatisticsCreator::Create(database_functions, general_database_functions);

  return BashAnalyzerObjectPtr(new BashAnalyzerObject(dusc, scripts_interface, system_interface));
}

void BashAnalyzerObject::Analyze() {
  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::BashAnalyzerObject::Analyze: Function call";

  auto today = GetCurrentDate();

  scripts_interface_->CreateDailySystemStatistics();
  daily_user_statistics_creator_->CreateStatistics(today);
}

BashAnalyzerObject::BashAnalyzerObject(detail::DailyUserStatisticsCreatorInterfacePtr daily_user_statistics_creator,
                                       ::bash::domain::detail::ScriptsInterfacePtr scripts_interface,
                                       detail::SystemInterfacePtr system_interface) :
daily_user_statistics_creator_(daily_user_statistics_creator),
scripts_interface_(scripts_interface),
system_interface_(system_interface) {
}

::type::Date BashAnalyzerObject::GetCurrentDate() const {
  BOOST_LOG_TRIVIAL(debug) << "bash::analyzer::BashAnalyzerObject::GetCurrentDate: Function call";
  time_t t = system_interface_->Time(nullptr);
  struct tm *now = system_interface_->LocalTime(&t);

  return ::type::Date::Create(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
}

}

}
