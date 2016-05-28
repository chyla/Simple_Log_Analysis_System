#ifndef PATLMS_TYPE_BASH_LOG_ENTRY_H
#define PATLMS_TYPE_BASH_LOG_ENTRY_H

#include <vector>

#include <patlms/type/timestamp.h>

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

#endif /* PATLMS_TYPE_BASH_LOG_ENTRY_H */
