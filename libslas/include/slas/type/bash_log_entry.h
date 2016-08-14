#pragma once

#include <vector>

#include <slas/type/timestamp.h>

namespace type
{

struct BashLogEntry {
  long long id;
  std::string agent_name;
  Timestamp utc_time;
  unsigned user_id;
  std::string command;
};

typedef std::vector<BashLogEntry> BashLogs;

}
