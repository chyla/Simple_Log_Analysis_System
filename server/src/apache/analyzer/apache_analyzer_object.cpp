/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "apache_analyzer_object.h"

#include <boost/log/trivial.hpp>

#include "detail/prepare_statistics_analyzer_object.h"

namespace apache
{

namespace analyzer
{

ApacheAnalyzerObjectPtr ApacheAnalyzerObject::Create(::apache::database::DatabaseFunctionsPtr database_functions) {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::ApacheAnalyzerObject::Create: Function call";

  return ApacheAnalyzerObjectPtr(new ApacheAnalyzerObject(database_functions));
}

void ApacheAnalyzerObject::Analyze() {
  BOOST_LOG_TRIVIAL(debug) << "apache::analyzer::ApacheAnalyzerObject::Analyze: Function call";

  auto statistics = detail::PrepareStatisticsAnalyzerObject::Create(database_functions_);

  statistics->Prepare();
}

ApacheAnalyzerObject::ApacheAnalyzerObject(::apache::database::DatabaseFunctionsPtr database_functions) :
database_functions_(database_functions) {
}

}

}
