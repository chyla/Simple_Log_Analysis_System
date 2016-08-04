/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "src/database/type/row_id.h"
#include "src/database/type/rows_count.h"

#include <vector>

namespace bash
{

namespace database
{

namespace detail
{

namespace entity
{

struct CommandStatistic {
  ::database::type::RowId id;
  ::database::type::RowId agent_name_id;
  ::database::type::RowId command_id;
  ::database::type::RowId begin_date_id;
  ::database::type::RowId end_date_id;
  ::database::type::RowsCount summary;
};

typedef std::vector<CommandStatistic> CommandsStatistics;

}

}

}

}
