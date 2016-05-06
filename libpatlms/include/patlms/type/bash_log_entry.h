#ifndef PATLMS_TYPE_BASH_LOG_ENTRY_H
#define PATLMS_TYPE_BASH_LOG_ENTRY_H

#include <patlms/type/time.h>

namespace type
{

struct BashLogEntry {
  std::string agent_name;
  Time utc_time;
  unsigned user_id;
  std::string command;
};

}

#endif /* PATLMS_TYPE_BASH_LOG_ENTRY_H */
