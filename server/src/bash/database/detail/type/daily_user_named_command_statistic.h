/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "src/bash/database/type/command_name.h"
#include "src/database/type/row_id.h"
#include "src/database/type/rows_count.h"

#include <vector>

namespace bash
{

namespace database
{

namespace detail
{

namespace type
{

struct DailyUserNamedCommandStatistic {
  ::database::type::RowId command_id;
  ::bash::database::type::CommandName command_name;
  ::database::type::RowsCount summary;
};

typedef std::vector<DailyUserNamedCommandStatistic> DailyUserNamedCommandsStatistics;

}

}

}

}
