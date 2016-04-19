#ifndef PATLMS_TYPE_BASH_LOG_ENTRY_H
#define PATLMS_TYPE_BASH_LOG_ENTRY_H

#include "src/type/time.h"

namespace type
{

struct BashLogEntry {
  std::string hostname;
  Time utc_time;
  unsigned user_id;
  std::string command;
};

}

#endif /* PATLMS_TYPE_BASH_LOG_ENTRY_H */
