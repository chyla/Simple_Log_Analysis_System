#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <poll.h>
#include <ctime>

#include <memory>

namespace network
{

namespace detail
{

class SystemInterface {
 public:
  virtual ~SystemInterface();

  virtual int Connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen) = 0;

  virtual int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) = 0;

  virtual int Unlink(const char *pathname) = 0;

  virtual int Socket(int domain, int type, int protocol) = 0;

  virtual int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) = 0;

  virtual int Listen(int sockfd, int backlog) = 0;

  virtual int Chmod(const char *pathname, mode_t mode) = 0;

  virtual int Close(int socket) = 0;

  virtual int Poll(struct pollfd *fds, nfds_t nfds, int timeout) = 0;

  virtual ssize_t Recv(int sockfd, void *buf, size_t len, int flags) = 0;

  virtual ssize_t Send(int sockfd, const void *buf, size_t len, int flags) = 0;

  virtual int Getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen) = 0;

  virtual time_t Time(time_t *t) = 0;

  virtual struct tm *GMTime(const time_t *timep) = 0;
};

typedef std::shared_ptr<SystemInterface> SystemInterfacePtr;

}

}
