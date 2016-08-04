/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include <patlms/type/date.h>

#include "src/database/type/row_id.h"
#include "src/database/type/agent_name.h"
#include "src/database/type/classification.h"
#include "src/bash/database/detail/entity/system_user.h"

#include <vector>

namespace bash
{

namespace domain
{

namespace type
{

struct DailyUserStatistic {
  ::database::type::RowId id;
  ::database::type::AgentName agent_name;
  ::bash::database::type::UID uid;
  ::type::Date date;
  ::database::type::Classification classification;
};

typedef std::vector<DailyUserStatistic> DailyUserStatistics;

}

}

}
