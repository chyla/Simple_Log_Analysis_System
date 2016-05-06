#ifndef SYSTEM_H
#define SYSTEM_H

#include "system_interface.h"

namespace bash
{

namespace detail
{

class System : public SystemInterface {
 public:
  virtual ~System();

  int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

  int Unlink(const char *pathname);

  int Socket(int domain, int type, int protocol);

  int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

  int Listen(int sockfd, int backlog);

  int Chmod(const char *pathname, mode_t mode);

  int Poll(struct pollfd *fds, nfds_t nfds, int timeout);

  ssize_t Recv(int sockfd, void *buf, size_t len, int flags);

  int Getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);

  time_t Time(time_t *t);

  struct tm *GMTime(const time_t *timep);
};

}

}

#endif /* SYSTEM_H */
