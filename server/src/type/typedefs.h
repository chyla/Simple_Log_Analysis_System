#ifndef PATLMS_TYPEDEFS_H
#define PATLMS_TYPEDEFS_H

#include <cstdint>

namespace type
{

typedef int64_t time_sec_t;

struct BashLogEntry;
typedef std::vector<type::BashLogEntry> BashLogs;

}

#endif /* PATLMS_TYPEDEFS_H */
