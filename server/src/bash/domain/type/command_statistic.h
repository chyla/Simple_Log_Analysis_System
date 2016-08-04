/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include <patlms/type/date.h>

#include "src/database/type/row_id.h"
#include "src/database/type/rows_count.h"
#include "src/bash/database/type/command_name.h"

#include <vector>

namespace bash
{

namespace domain
{

namespace type
{

struct CommandStatistic {
  ::database::type::RowId command_name_id;
  ::bash::database::type::CommandName command_name;
  ::database::type::RowsCount summary_count;
};

typedef std::vector<CommandStatistic> CommandsStatistics;

}

}

}
