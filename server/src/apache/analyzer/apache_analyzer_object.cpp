/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "apache_analyzer_object.h"

#include <boost/log/trivial.hpp>

#include <slas/util/distance.h>
#include "detail/session_length.h"
#include "detail/prepare_statistics_analyzer_object.h"
#include "detail/knn_analyzer_object.h"
#include "src/apache/notifier/type/apache_notifier_message.h"

namespace apache
{

namespace analyzer
{

ApacheAnalyzerObjectPtr ApacheAnalyzerObject::Create(::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                                     ::apache::database::DatabaseFunctionsPtr database_functions,
                                                     ::notifier::detail::NotifierInterfacePtr notifier) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::ApacheAnalyzerObject::Create: Function call";
  auto system_interface = detail::System::Create();

  return Create(general_database_functions, database_functions, notifier, system_interface);
}

ApacheAnalyzerObjectPtr ApacheAnalyzerObject::Create(::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                                     ::apache::database::DatabaseFunctionsPtr database_functions,
                                                     ::notifier::detail::NotifierInterfacePtr notifier,
                                                     detail::SystemInterfacePtr system_interface) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::ApacheAnalyzerObject::Create: Function call";

  return ApacheAnalyzerObjectPtr(new ApacheAnalyzerObject(general_database_functions, database_functions, notifier, system_interface));
}

void ApacheAnalyzerObject::Analyze() {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::ApacheAnalyzerObject::Analyze: Function call";

  auto statistics = detail::PrepareStatisticsAnalyzerObject::Create(database_functions_);
  auto knn_analyzer = detail::KnnAnalyzerObject::Create(general_database_functions_,
                                                        database_functions_);

  auto now = GetCurrentTimestamp();

  if (ShouldRun(now)) {
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::ApacheAnalyzerObject::Analyze: ShouldRun: true";
    statistics->Prepare(now);
    knn_analyzer->Analyze();

    if (knn_analyzer->IsAnomalyDetected()) {
      auto summary = knn_analyzer->GetAnalyzeSummary();
      auto message = ::apache::notifier::type::ApacheNotifierMessage::Create(summary,
                                                                             general_database_functions_,
                                                                             database_functions_);
      notifier_->AddMessages({message});
    }

    database_functions_->SetLastRun(now);
  }
  else {
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::ApacheAnalyzerObject::Analyze: ShouldRun: false";
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::ApacheAnalyzerObject::Analyze: Not running";
  }
}

ApacheAnalyzerObject::ApacheAnalyzerObject(::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                           ::apache::database::DatabaseFunctionsPtr database_functions,
                                           ::notifier::detail::NotifierInterfacePtr notifier,
                                           detail::SystemInterfacePtr system_interface) :
general_database_functions_(general_database_functions),
database_functions_(database_functions),
notifier_(notifier),
system_interface_(system_interface) {
}

bool ApacheAnalyzerObject::ShouldRun(const ::type::Timestamp &now) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::ApacheAnalyzerObject::ShouldRun: Function call";

  if (database_functions_->IsLastRunSet() == false) {
    BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::ApacheAnalyzerObject::ShouldRun: Last run not set, returning true";
    return true;
  }

  auto last_run = database_functions_->GetLastRun();
  return (last_run.GetDate() != now.GetDate()) ||
      (::util::Distance(last_run.GetTime(), now.GetTime()) > detail::SESSION_LENGTH);
}

::type::Timestamp ApacheAnalyzerObject::GetCurrentTimestamp() const {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::detail::KnnAnalyzerObject::GetCurrentTimestamp: Function call";
  time_t t = system_interface_->Time(nullptr);
  struct tm *now = system_interface_->LocalTime(&t);

  return ::type::Timestamp::Create(::type::Time::Create(now->tm_hour, now->tm_min, now->tm_sec),
                                   ::type::Date::Create(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900));
}

}

}
