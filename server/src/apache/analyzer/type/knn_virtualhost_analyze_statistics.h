/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef APACHE_ANALYZER_TYPE_KNN_VIRTUALHOST_ANALYZE_STATISTICS_H
#define APACHE_ANALYZER_TYPE_KNN_VIRTUALHOST_ANALYZE_STATISTICS_H

#include "src/database/type/row_id.h"

#include <vector>

namespace apache
{

namespace analyzer
{

namespace type
{

struct KnnVirtualhostAnalyzeStatistics {

  KnnVirtualhostAnalyzeStatistics() :
  agent_id(0),
  virtualhost_id(0) {
  }

  ::database::type::RowId agent_id;
  ::database::type::RowId virtualhost_id;
  std::vector<::database::type::RowId> anomaly_sessions_ids;
};

}

}

}

#endif /* APACHE_ANALYZER_TYPE_KNN_VIRTUALHOST_ANALYZE_STATISTICS_H */
