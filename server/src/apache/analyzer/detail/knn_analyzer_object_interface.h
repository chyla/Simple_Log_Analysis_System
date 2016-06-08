/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_APACHE_ANALYZER_DETAIL_KNN_ANALYZER_OBJECT_INTERFACE_H
#define SRC_APACHE_ANALYZER_DETAIL_KNN_ANALYZER_OBJECT_INTERFACE_H

#include <memory>

#include <patlms/type/timestamp.h>

namespace apache
{

namespace analyzer
{

namespace detail
{

class KnnAnalyzerObjectInterface;
typedef std::shared_ptr<KnnAnalyzerObjectInterface> KnnAnalyzerObjectInterfacePtr;

class KnnAnalyzerObjectInterface {
 public:
  virtual ~KnnAnalyzerObjectInterface() = default;

  virtual void Analyze(const ::type::Timestamp &now) = 0;
};

}

}

}

#endif /* SRC_APACHE_ANALYZER_DETAIL_KNN_ANALYZER_OBJECT_INTERFACE_H */
