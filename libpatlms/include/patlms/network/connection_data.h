#ifndef CONNECTION_DATA_H
#define CONNECTION_DATA_H

#include <sys/socket.h>
#include <cstring>

namespace network
{

struct ConnectionData {

  ConnectionData()
  : socket(0),
  addr_size(sizeof (struct sockaddr_storage)) {
    (void) memset(&addr, 0, sizeof (struct sockaddr_storage));
  }

  int socket;
  socklen_t addr_size;
  struct sockaddr_storage addr;
};

}

#endif /* CONNECTION_DATA_H */
