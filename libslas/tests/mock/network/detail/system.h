#pragma once

#include <gmock/gmock.h>
#include <sys/socket.h>

#include <slas/network/detail/system_interface.h>

namespace mock
{

namespace network
{

namespace detail
{

class System : public ::network::detail::SystemInterface {
 public:
  MOCK_METHOD1(Unlink, int(const char *pathname));

  MOCK_METHOD1(Unlink, int(int domain));

  MOCK_METHOD3(Connect, int(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen));

  MOCK_METHOD3(Socket, int(int domain, int type, int protocol));

  MOCK_METHOD3(Bind, int(int sockfd, const struct sockaddr *addr, socklen_t addrlen));

  MOCK_METHOD2(Listen, int(int sockfd, int backlog));

  MOCK_METHOD2(Chmod, int(const char *pathname, mode_t mode));

  MOCK_METHOD1(Close, int(int socket));

  MOCK_METHOD3(Poll, int(struct pollfd *fds, nfds_t nfds, int timeout));

  MOCK_METHOD4(Recv, ssize_t(int sockfd, void *buf, size_t len, int flags));

  MOCK_METHOD4(Send, ssize_t(int sockfd, const void *buf, size_t len, int flags));

  MOCK_METHOD3(Accept, int(int sockfd, struct sockaddr *addr, socklen_t *addrlen));

  MOCK_METHOD5(Getsockopt, int(int sockfd, int level, int optname, void *optval, socklen_t *optlen));

  MOCK_METHOD5(Setsockopt, int(int sockfd, int level, int optname, void *optval, socklen_t *optlen));

  MOCK_METHOD1(Time, time_t(time_t *t));

  MOCK_METHOD1(GMTime, struct tm*(const time_t *timep));
};

}

}

}
