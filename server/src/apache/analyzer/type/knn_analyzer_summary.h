/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef APACHE_ANALYZER_TYPE_KNN_ANALYZER_SUMMARY_H
#define APACHE_ANALYZER_TYPE_KNN_ANALYZER_SUMMARY_H

#include "knn_virtualhost_analyze_statistics.h"

namespace apache
{

namespace analyzer
{

namespace type
{

typedef std::vector<KnnVirtualhostAnalyzeStatistics> KnnAnalyzerSummary;

}

}

}

#endif /* APACHE_ANALYZER_TYPE_KNN_ANALYZER_SUMMARY_H */
