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

#include "prepare_statistics/nearest_neighbours_table.h"
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

  void Analyze();

  bool IsAnomalyDetected() const override;

  ::apache::analyzer::type::KnnAnalyzerSummary GetAnalyzeSummary() const override;

 private:
  KnnAnalyzerObject(::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                    ::apache::database::DatabaseFunctionsPtr apache_database_functions);

  void AnalyzeVirtualhost(const ::database::type::RowId &agent_name_id,
                          const ::database::type::RowId &virtualhost_name_id);

  void AnalyzeSessions(const ::database::type::RowId &agent_name_id,
                       const ::database::type::RowId &virtualhost_name_id,
                       unsigned limit,
                       ::database::type::RowsCount offset);

  void AnalyzeSessionsWithLearningSet(const ::database::type::RowId &agent_name_id,
                                      const ::database::type::RowId &virtualhost_name_id,
                                      unsigned limit,
                                      ::database::type::RowsCount offset,
                                      const ::apache::type::ApacheSessionEntry &session_to_analyze);

  bool IsSessionAnomaly();
  double Distance(const ::apache::type::ApacheSessionEntry &a, const ::apache::type::ApacheSessionEntry &b) const;

  ::type::Timestamp GetCurrentTimestamp() const;
  ::type::Timestamp GetLastAnalyzeTimestamp(const ::type::Timestamp &now) const;

  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;
  ::apache::database::DatabaseFunctionsPtr apache_database_functions_;

  prepare_statistics::NearestNeighboursTable neighbours_table_;

  bool is_anomaly_detected_;
  ::apache::analyzer::type::KnnAnalyzerSummary analyze_summary_;
};

}

}

}

#endif /* SRC_APACHE_ANALYZER_DETAIL_KNN_ANALYZER_OBJECT_H */
