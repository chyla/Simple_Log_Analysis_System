#ifndef PATLMS_TYPEDEFS_H
#define PATLMS_TYPEDEFS_H

#include <cstdint>

namespace type
{

struct BashLogEntry;
typedef std::vector<type::BashLogEntry> BashLogs;

struct ApacheLogEntry;
typedef std::vector<ApacheLogEntry> ApacheLogs;

}

#endif /* PATLMS_TYPEDEFS_H */
