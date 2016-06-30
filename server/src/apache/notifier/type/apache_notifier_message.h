/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef APACHE_NOTIFIER_TYPE_APACHE_NOTIFIER_MESSAGE_H
#define APACHE_NOTIFIER_TYPE_APACHE_NOTIFIER_MESSAGE_H

#include "src/notifier/type/notifier_message_interface.h"
#include "src/apache/analyzer/type/knn_analyzer_summary.h"
#include "src/database/detail/general_database_functions_interface.h"
#include "src/apache/database/database_functions.h"

namespace apache
{

namespace notifier
{

namespace type
{

class ApacheNotifierMessage;
typedef std::shared_ptr<ApacheNotifierMessage> ApacheNotifierMessagePtr;

class ApacheNotifierMessage : public ::notifier::type::NotifierMessageInterface {
 public:
  virtual ~ApacheNotifierMessage() = default;

  static ApacheNotifierMessagePtr Create(::apache::analyzer::type::KnnAnalyzerSummary summary,
                                         ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                         ::apache::database::DatabaseFunctionsPtr apache_database_functions);

  std::string GetModuleName();
  std::string GetDetectionResults();

 private:
  ApacheNotifierMessage(::apache::analyzer::type::KnnAnalyzerSummary summary,
                        ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                        ::apache::database::DatabaseFunctionsPtr apache_database_functions);

  const ::apache::analyzer::type::KnnAnalyzerSummary summary_;
  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;
  ::apache::database::DatabaseFunctionsPtr apache_database_functions_;
};

}

}

}

#endif /* APACHE_NOTIFIER_TYPE_APACHE_NOTIFIER_MESSAGE_H */
