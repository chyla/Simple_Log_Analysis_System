/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_APACHE_ANALYZER_APACHE_ANALYZER_OBJECT_H
#define SRC_APACHE_ANALYZER_APACHE_ANALYZER_OBJECT_H

#include <memory>

#include "src/analyzer/analyzer_object_interface.h"
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

  static ApacheAnalyzerObjectPtr Create(::apache::database::DatabaseFunctionsPtr database_functions);

  void Analyze() override;

 private:
  ApacheAnalyzerObject(::apache::database::DatabaseFunctionsPtr database_functions);

  ::apache::database::DatabaseFunctionsPtr database_functions_;
};

}

}

#endif /* SRC_APACHE_ANALYZER_APACHE_ANALYZER_OBJECT_H */
