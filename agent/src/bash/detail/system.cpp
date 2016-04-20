#include "system.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctime>

namespace bash
{

namespace detail
{

System::~System() {
}

int System::Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
  return accept(sockfd, addr, addrlen);
}

int System::Unlink(const char *pathname) {
  return unlink(pathname);
}

int System::Socket(int domain, int type, int protocol) {
  return socket(domain, type, protocol);
}

int System::Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  return bind(sockfd, addr, addrlen);
}

int System::Listen(int sockfd, int backlog) {
  return listen(sockfd, backlog);
}

int System::Chmod(const char *pathname, mode_t mode) {
  return chmod(pathname, mode);
}

int System::Poll(struct pollfd *fds, nfds_t nfds, int timeout) {
  return poll(fds, nfds, timeout);
}

ssize_t System::Recv(int sockfd, void *buf, size_t len, int flags) {
  return recv(sockfd, buf, len, flags);
}

int System::Getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen) {
  return getsockopt(sockfd, level, optname, optval, optlen);
}

time_t System::Time(time_t *t) {
  return time(t);
}

struct tm* System::GMTime(const time_t *timep) {
  return gmtime(timep);
}

int System::Gethostname(char *name, size_t len) {
  return gethostname(name, len);
}

}

}
