/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_APACHE_ANALYZER_APACHE_ANALYZER_OBJECT_H
#define SRC_APACHE_ANALYZER_APACHE_ANALYZER_OBJECT_H

#include "src/analyzer/analyzer_object_interface.h"

#include <memory>

#include <patlms/type/timestamp.h>
#include "src/database/detail/general_database_functions_interface.h"
#include "src/apache/database/database_functions.h"
#include "detail/system.h"
#include "src/notifier/detail/notifier_interface.h"

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
                                        ::apache::database::DatabaseFunctionsPtr database_functions,
                                        ::notifier::detail::NotifierInterfacePtr notifier);

  static ApacheAnalyzerObjectPtr Create(::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                        ::apache::database::DatabaseFunctionsPtr database_functions,
                                        ::notifier::detail::NotifierInterfacePtr notifier,
                                        detail::SystemInterfacePtr system_interface);

  void Analyze() override;

 private:
  ApacheAnalyzerObject(::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                       ::apache::database::DatabaseFunctionsPtr database_functions,
                       ::notifier::detail::NotifierInterfacePtr notifier,
                       detail::SystemInterfacePtr system_interface);

  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;
  ::apache::database::DatabaseFunctionsPtr database_functions_;
  ::notifier::detail::NotifierInterfacePtr notifier_;
  detail::SystemInterfacePtr system_interface_;

  bool ShouldRun(const ::type::Timestamp &now);
  ::type::Timestamp GetCurrentTimestamp() const;
};

}

}

#endif /* SRC_APACHE_ANALYZER_APACHE_ANALYZER_OBJECT_H */
