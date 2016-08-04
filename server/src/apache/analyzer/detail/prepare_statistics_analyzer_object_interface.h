/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

namespace apache
{

namespace analyzer
{

namespace detail
{

class PrepareStatisticsAnalyzerObjectInterface {
 public:
  virtual ~PrepareStatisticsAnalyzerObjectInterface() = default;

  virtual void Prepare(const ::type::Timestamp &now) = 0;
};

}

}

}
