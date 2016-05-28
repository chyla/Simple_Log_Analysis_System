#ifndef SRC_APACHE_TYPE_APACHE_SESSION_ENTRY_H
#define SRC_APACHE_TYPE_APACHE_SESSION_ENTRY_H

#include <string>
#include <vector>
#include <patlms/type/time.h>

namespace apache
{

namespace type
{

struct ApacheSessionEntry {
  long long id;
  std::string agent_name;
  std::string virtualhost;
  std::string client_ip;
  ::type::Time session_start;
  long session_length;
  long bandwidth_usage;
  long requests_count;
  int error_percentage;
  std::string useragent;
  bool is_anomaly;
};

typedef std::vector<ApacheSessionEntry> ApacheSessions;

}

}

#endif /* SRC_APACHE_TYPE_APACHE_SESSION_ENTRY_H */
