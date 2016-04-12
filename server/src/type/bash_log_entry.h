#ifndef PATLMS_TYPE_BASH_LOG_ENTRY_H
#define PATLMS_TYPE_BASH_LOG_ENTRY_H

#include "src/type/typedefs.h"

namespace type
{

struct BashLogEntry {
  std::string hostname;
  time_sec_t time;
  unsigned user_id;
  std::string command;
};

}

#endif /* PATLMS_TYPE_BASH_LOG_ENTRY_H */
