/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_APACHE_ANALYZER_DETAIL_KNN_ANALYZER_OBJECT_H
#define SRC_APACHE_ANALYZER_DETAIL_KNN_ANALYZER_OBJECT_H

#include "knn_analyzer_object_interface.h"

#include <array>
#include <memory>
#include <utility>

#include <patlms/type/timestamp.h>

#include "system_interface.h"
#include "src/database/detail/general_database_functions_interface.h"
#include "src/apache/database/database_functions.h"
#include "src/database/type/agent_name.h"
#include "src/database/type/virtualhost_name.h"

namespace apache
{

namespace analyzer
{

namespace detail
{

class KnnAnalyzerObject;
typedef std::shared_ptr<KnnAnalyzerObject> KnnAnalyzerObjectPtr;

class KnnAnalyzerObject : public KnnAnalyzerObjectInterface {
 public:
  virtual ~KnnAnalyzerObject() = default;
  
  static KnnAnalyzerObjectPtr Create(::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                     ::apache::database::DatabaseFunctionsPtr apache_database_functions);

  static KnnAnalyzerObjectPtr Create(detail::SystemInterfacePtr system_interface,
                                     ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                     ::apache::database::DatabaseFunctionsPtr apache_database_functions);

  void Analyze();

 private:
  KnnAnalyzerObject(detail::SystemInterfacePtr system_interface,
                    ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                    ::apache::database::DatabaseFunctionsPtr apache_database_functions);

  void AnalyzeVirtualhost(const ::database::type::AgentName &agent_name,
                          const ::database::type::VirtualhostName &virtualhost_name,
                          const ::type::Timestamp &last_analyze_timestamp,
                          const ::type::Timestamp &now);

  void AnalyzeSessions(const ::database::type::AgentName &agent_name,
                       const ::database::type::VirtualhostName &virtualhost_name,
                       const ::type::Timestamp &from,
                       const ::type::Timestamp &to,
                       unsigned limit,
                       ::database::type::RowsCount offset);

  void AnalyzeSessionsWithLearningSet(const ::database::type::RowId &agent_name_id,
                                      const ::database::type::RowId &virtualhost_name_id,
                                      unsigned limit,
                                      ::database::type::RowsCount offset,
                                      const ::apache::type::ApacheSessionEntry &session_to_analyze);

  double Distance(const ::apache::type::ApacheSessionEntry &a, const ::apache::type::ApacheSessionEntry &b) const;
  
  ::type::Timestamp GetCurrentTimestamp() const;
  ::type::Timestamp GetLastAnalyzeTimestamp(const ::type::Timestamp &now) const;

  detail::SystemInterfacePtr system_interface_;
  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;
  ::apache::database::DatabaseFunctionsPtr apache_database_functions_;
  
  typedef std::pair<double, ::database::type::RowId> DistanceSessionId;
  std::array<DistanceSessionId, 3> distance_table_;
};

}

}

}

#endif /* SRC_APACHE_ANALYZER_DETAIL_KNN_ANALYZER_OBJECT_H */
