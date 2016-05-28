#ifndef APACHE_LOG_ENTRY_H
#define APACHE_LOG_ENTRY_H

#include <vector>

#include <patlms/type/timestamp.h>

namespace type
{

struct ApacheLogEntry {
  long long id;
  std::string agent_name;
  std::string virtualhost;
  std::string client_ip;
  Timestamp time;
  std::string request;
  int status_code;
  int bytes;
  std::string user_agent;
};

typedef std::vector<ApacheLogEntry> ApacheLogs;

}

#endif /* APACHE_LOG_ENTRY_H */
