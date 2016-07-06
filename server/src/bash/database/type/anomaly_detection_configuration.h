/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef BASH_DATABASE_TYPE_ANOMALY_DETECTION_CONFIGURATION_H
#define BASH_DATABASE_TYPE_ANOMALY_DETECTION_CONFIGURATION_H

#include <patlms/type/date.h>

#include "src/database/type/row_id.h"

#include <vector>

namespace bash
{

namespace database
{

namespace type
{

struct AnomalyDetectionConfiguration {
  ::database::type::RowId id;
  ::database::type::RowId agent_name_id;
  ::type::Date begin_date;
  ::type::Date end_date;
};

typedef std::vector<AnomalyDetectionConfiguration> AnomalyDetectionConfigurations;

}

}

}

#endif /* BASH_DATABASE_TYPE_ANOMALY_DETECTION_CONFIGURATION_H */
