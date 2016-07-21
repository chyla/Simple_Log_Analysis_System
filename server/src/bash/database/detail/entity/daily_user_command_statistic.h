/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef DAILY_USER_COMMAND_STATISTIC_H
#define DAILY_USER_COMMAND_STATISTIC_H

#include "src/database/type/row_id.h"
#include "src/database/type/rows_count.h"

namespace bash
{

namespace database
{

namespace detail
{

namespace entity
{

struct DailyUserCommandStatistic {
  ::database::type::RowId id;
  ::database::type::RowId daily_user_statistic_id;
  ::database::type::RowId command_id;
  ::database::type::RowsCount summary;
};

}

}

}

}

#endif /* DAILY_USER_STATISTIC_H */