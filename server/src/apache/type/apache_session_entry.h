#pragma once

#include <string>
#include <vector>

#include <slas/type/timestamp.h>

#include "src/database/type/row_id.h"
#include "src/database/type/agent_name.h"
#include "src/database/type/virtualhost_name.h"
#include "src/database/type/rows_count.h"
#include "src/database/type/classification.h"

namespace apache
{

namespace type
{

struct ApacheSessionEntry {
  ::database::type::RowId id;
  ::database::type::AgentName agent_name;
  ::database::type::VirtualhostName virtualhost;
  std::string client_ip;
  ::type::Timestamp session_start;
  long long session_length;
  long long bandwidth_usage;
  ::database::type::RowsCount requests_count;
  ::database::type::RowsCount errors_count;
  double error_percentage;
  std::string useragent;
  ::database::type::Classification classification;
};

typedef std::vector<ApacheSessionEntry> ApacheSessions;

}

}
