/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "src/database/type/row_id.h"

#include <vector>

namespace bash
{

namespace database
{

namespace detail
{

namespace entity
{

struct AnomalyDetectionConfiguration {
  ::database::type::RowId id;
  ::database::type::RowId agent_name_id;
  ::database::type::RowId begin_date_id;
  ::database::type::RowId end_date_id;
  bool changed;
};

typedef std::vector<AnomalyDetectionConfiguration> AnomalyDetectionConfigurations;

}

}

}

}
