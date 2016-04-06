#include "receive_messages.h"

#include <string>
#include <sstream>
#include <cstdio>
#include <cstring>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/stat.h>

#define SOCK_PATH "/tmp/bash-mod.sock"


namespace bash
{

void receive_messages(objects::BashProxy *bash_proxy)
{
  int fd;
  struct sockaddr_un saddr;

  saddr.sun_family = AF_UNIX;
  strcpy(saddr.sun_path, SOCK_PATH);

  unlink(SOCK_PATH);

  fd = socket(PF_UNIX, SOCK_STREAM, 0);
  bind(fd, (struct sockaddr *) &saddr, sizeof(struct sockaddr_un));
  listen(fd, 20);

  chmod(SOCK_PATH, 0622);

  while (true) {
    struct sockaddr_storage their_addr;
    socklen_t len = sizeof(struct sockaddr_storage);
    int new_fd = accept(fd, (struct sockaddr*) &their_addr, &len);

    struct ucred cr;
    socklen_t cr_len = sizeof(struct ucred);
    getsockopt(new_fd, SOL_SOCKET, SO_PEERCRED, &cr, &cr_len);

    ssize_t r = 0;
    char buf[1024];

    r = recv(new_fd, buf, 1024, 0);
    if (r > 0) {
	  buf[r] = 0;
      std::stringstream s;
      s << "user: " << cr.uid << ", cmd: " << buf;
      bash_proxy->AddLogEntry(s.str());
    }
  }
}

}

