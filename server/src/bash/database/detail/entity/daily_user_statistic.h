/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "src/database/type/row_id.h"
#include "src/database/type/classification.h"

#include <vector>

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
  ::database::type::Classification classification;
};

typedef std::vector<DailyUserStatistic> DailyUserStatistics;

}

}

}

}
