/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef DAILY_SYSTEM_STATISTICS_H
#define DAILY_SYSTEM_STATISTICS_H

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

struct DailySystemStatistic {
  ::database::type::RowId agent_name_id;
  ::database::type::RowId date_id;
  ::database::type::RowId command_id;
  ::database::type::RowsCount summary;
};

}

}

}

}

#endif /* DAILY_STATISTICS_H */
