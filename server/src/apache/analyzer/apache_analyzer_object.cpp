/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "apache_analyzer_object.h"

#include <boost/log/trivial.hpp>

#include "detail/prepare_statistics_analyzer_object.h"
#include "detail/knn_analyzer_object.h"

namespace apache
{

namespace analyzer
{

ApacheAnalyzerObjectPtr ApacheAnalyzerObject::Create(::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                                     ::apache::database::DatabaseFunctionsPtr database_functions) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::ApacheAnalyzerObject::Create: Function call";

  return ApacheAnalyzerObjectPtr(new ApacheAnalyzerObject(general_database_functions, database_functions));
}

void ApacheAnalyzerObject::Analyze() {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::ApacheAnalyzerObject::Analyze: Function call";

  auto statistics = detail::PrepareStatisticsAnalyzerObject::Create(database_functions_);
  auto knn_analyzer = detail::KnnAnalyzerObject::Create(general_database_functions_,
                                                        database_functions_);

  statistics->Prepare();
  knn_analyzer->Analyze();
}

ApacheAnalyzerObject::ApacheAnalyzerObject(::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                           ::apache::database::DatabaseFunctionsPtr database_functions) :
general_database_functions_(general_database_functions),
database_functions_(database_functions) {
}

}

}
