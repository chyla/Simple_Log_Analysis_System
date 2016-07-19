/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef BASH_ANALYZER_DETAIL_DAILY_USER_STATISTICS_CREATOR_INTERFACE_H
#define BASH_ANALYZER_DETAIL_DAILY_USER_STATISTICS_CREATOR_INTERFACE_H

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

#endif /* BASH_ANALYZER_DETAIL_DAILY_USER_STATISTICS_CREATOR_INTERFACE_H */
