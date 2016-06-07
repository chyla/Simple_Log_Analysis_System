/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_APACHE_ANALYZER_APACHE_ANALYZER_OBJECT_H
#define SRC_APACHE_ANALYZER_APACHE_ANALYZER_OBJECT_H

#include "src/analyzer/analyzer_object_interface.h"

#include <memory>

#include "src/database/detail/general_database_functions_interface.h"
#include "src/apache/database/database_functions.h"

namespace apache
{

namespace analyzer
{

class ApacheAnalyzerObject;
typedef std::shared_ptr<ApacheAnalyzerObject> ApacheAnalyzerObjectPtr;

class ApacheAnalyzerObject : public ::analyzer::AnalyzerObjectInterface {
 public:
  virtual ~ApacheAnalyzerObject() = default;

  static ApacheAnalyzerObjectPtr Create(::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                        ::apache::database::DatabaseFunctionsPtr database_functions);

  void Analyze() override;

 private:
  ApacheAnalyzerObject(::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                       ::apache::database::DatabaseFunctionsPtr database_functions);

  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;
  ::apache::database::DatabaseFunctionsPtr database_functions_;
};

}

}

#endif /* SRC_APACHE_ANALYZER_APACHE_ANALYZER_OBJECT_H */
