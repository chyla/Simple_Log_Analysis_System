/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_APACHE_ANALYZER_DETAIL_PREPARE_STATISTICS_ANALYZER_OBJECT_INTERFACE_H
#define SRC_APACHE_ANALYZER_DETAIL_PREPARE_STATISTICS_ANALYZER_OBJECT_INTERFACE_H

namespace apache
{

namespace analyzer
{

namespace detail
{

class PrepareStatisticsAnalyzerObjectInterface {
 public:
  virtual ~PrepareStatisticsAnalyzerObjectInterface() = default;

  virtual void Prepare() = 0;
};

}

}

}

#endif /* SRC_APACHE_ANALYZER_DETAIL_PREPARE_STATISTICS_ANALYZER_OBJECT_INTERFACE_H */
