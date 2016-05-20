#ifndef APACHE_SESSION_ENTRY_H
#define APACHE_SESSION_ENTRY_H

#include <string>
#include <vector>

namespace analyzer
{

struct ApacheSessionEntry
{
  std::string agent_name;
  std::string virtualhost;
  std::string client_ip;
  long session_length;
  long bandwidth_usage;
  long requests_count;
  int error_percentage;
  std::string error_percentage;
};

typedef std::vector<ApacheSessionEntry> ApacheSessions;

}

#endif /* APACHE_SESSION_ENTRY_H */
