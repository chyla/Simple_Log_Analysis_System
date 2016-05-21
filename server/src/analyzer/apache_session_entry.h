#ifndef APACHE_SESSION_ENTRY_H
#define APACHE_SESSION_ENTRY_H

#include <string>
#include <vector>
#include <patlms/type/time.h>

namespace analyzer
{

struct ApacheSessionEntry {
  long long id;
  std::string agent_name;
  std::string virtualhost;
  std::string client_ip;
  type::Time session_start;
  long session_length;
  long bandwidth_usage;
  long requests_count;
  int error_percentage;
  std::string useragent;
};

typedef std::vector<ApacheSessionEntry> ApacheSessions;

}

#endif /* APACHE_SESSION_ENTRY_H */
