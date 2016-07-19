/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef DAILY_USER_STATISTIC_H
#define DAILY_USER_STATISTIC_H

#include "src/database/type/row_id.h"
#include "src/database/type/rows_count.h"
#include "src/database/type/classification.h"

namespace bash
{

namespace database
{

namespace detail
{

namespace entity
{

struct DailyUserStatistic {
  ::database::type::RowId id;
  ::database::type::RowId agent_name_id;
  ::database::type::RowId user_id;
  ::database::type::RowId date_id;
  ::database::type::RowId command_id;
  ::database::type::RowsCount summary;
  ::database::type::Classification classification;
};

}

}

}

}

#endif /* DAILY_USER_STATISTIC_H */
