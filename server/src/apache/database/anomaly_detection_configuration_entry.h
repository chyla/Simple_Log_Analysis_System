#ifndef ANOMALY_DETECTION_CONFIGURATION_ENTRY_H
#define ANOMALY_DETECTION_CONFIGURATION_ENTRY_H

#include <vector>

#include <patlms/type/time.h>

#include "src/database/types.h"

namespace apache
{

namespace database
{

struct AnomalyDetectionConfigurationEntry {
  ::database::RowId id;
  std::string agent_name;
  std::string virtualhost_name;
  type::Time begin_date;
  type::Time end_date;
};

typedef std::vector<AnomalyDetectionConfigurationEntry> AnomalyDetectionConfiguration;

}

}

#endif /* ANOMALY_DETECTION_CONFIGURATION_ENTRY_H */
