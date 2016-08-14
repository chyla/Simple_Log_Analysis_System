/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include <slas/type/date.h>

#include "src/database/type/row_id.h"
#include "src/database/type/agent_name.h"

#include <vector>

namespace bash
{

namespace domain
{

namespace type
{

struct AnomalyDetectionConfiguration {
  ::database::type::RowId id;
  ::database::type::RowId agent_name_id;
  ::database::type::AgentName agent_name;
  ::type::Date begin_date;
  ::type::Date end_date;
};

typedef std::vector<AnomalyDetectionConfiguration> AnomalyDetectionConfigurations;

}

}

}
