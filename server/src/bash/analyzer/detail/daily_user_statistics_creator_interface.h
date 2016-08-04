/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include <patlms/type/date.h>

#include <memory>

namespace bash
{

namespace analyzer
{

namespace detail
{

class DailyUserStatisticsCreatorInterface {
 public:
  virtual ~DailyUserStatisticsCreatorInterface() = default;

  virtual void CreateStatistics(const ::type::Date &today) = 0;
};

typedef std::shared_ptr<DailyUserStatisticsCreatorInterface> DailyUserStatisticsCreatorInterfacePtr;

}

}

}
