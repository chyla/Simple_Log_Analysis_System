#pragma once

#include <vector>

#include <slas/type/time.h>

#include "src/database/type/row_id.h"

namespace apache
{

namespace type
{

struct AnomalyDetectionConfigurationEntry {
  ::database::type::RowId id;
  std::string agent_name;
  std::string virtualhost_name;
  ::type::Date begin_date;
  ::type::Date end_date;
};

typedef std::vector<AnomalyDetectionConfigurationEntry> AnomalyDetectionConfiguration;

}

}
