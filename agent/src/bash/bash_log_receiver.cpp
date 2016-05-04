#include "bash_log_receiver.h"

#include <array>
#include <cstring>
#include <boost/log/trivial.hpp>

#include <sys/un.h>

#include <patlms/type/bash_log_entry.h>

#include "src/bash/exception/detail/cant_open_socket_exception.h"
#include "src/bash/exception/detail/loop_poll_exception.h"
#include "src/bash/exception/detail/loop_recv_exception.h"
#include "src/bash/exception/detail/loop_accept_exception.h"
#include "src/bash/exception/detail/loop_getsockopt_exception.h"
#include "src/bash/exception/detail/loop_time_exception.h"
#include "src/bash/exception/detail/loop_gmtime_exception.h"
#include "src/bash/exception/detail/loop_gethostname_exception.h"
#include "src/bash/exception/detail/socket_is_not_open_exception.h"
#include "detail/bash_dbus_thread_command.h"
#include "detail/system.h"

using namespace std;

namespace bash
{

shared_ptr<BashLogReceiver> BashLogReceiver::Create(shared_ptr<dbus::detail::BusInterface> bus,
                                                    shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread) {
  shared_ptr<bash::detail::SystemInterface> system(new detail::System());
  shared_ptr<BashLogReceiver> receiver(new BashLogReceiver(bus, dbus_thread, system));
  return receiver;
}

shared_ptr<BashLogReceiver> BashLogReceiver::Create(shared_ptr<dbus::detail::BusInterface> bus,
                                                    shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread,
                                                    shared_ptr<bash::detail::SystemInterface> system) {
  shared_ptr<BashLogReceiver> receiver(new BashLogReceiver(bus, dbus_thread, system));
  return receiver;
}

BashLogReceiver::~BashLogReceiver() {
}

void BashLogReceiver::OpenSocket(const std::string& socket_path) {
  int ret;
#warning TODO: Not portable
  struct sockaddr_un saddr;
  saddr.sun_family = AF_UNIX;
  saddr.sun_path[107] = '\0';
  strncpy(saddr.sun_path, socket_path.c_str(), 107);

  ret = system_->Unlink(socket_path.c_str());

  socket_fd_ = system_->Socket(PF_UNIX, SOCK_STREAM, 0);
  if (socket_fd_ < 0) {
    throw exception::detail::CantOpenSocketException();
  }

  ret = system_->Bind(socket_fd_, (struct sockaddr *) &saddr, sizeof (struct sockaddr_un));
  if (ret < 0) {
    throw exception::detail::CantOpenSocketException();
  }

  ret = system_->Listen(socket_fd_, 20);
  if (ret < 0) {
    throw exception::detail::CantOpenSocketException();
  }

  ret = system_->Chmod(socket_path.c_str(), 0622);
  if (ret < 0) {
    throw exception::detail::CantOpenSocketException();
  }
}

void BashLogReceiver::StartLoop() {
  std::shared_ptr<detail::BashProxy> bash_proxy(new detail::BashProxy(bus_));
  constexpr int timeout = 5000;
  char command[200] = {0};
  char hostname[200] = {0};
  int ret = 0, new_fd;

  if (socket_fd_ < 0) {
    throw exception::detail::SocketIsNotOpenException();
  }

  struct pollfd descriptors = {socket_fd_, POLLIN, 0};

  running_ = true;
  while (running_) {
    ret = system_->Poll(&descriptors, 1, timeout);
    BOOST_LOG_TRIVIAL(debug) << "bash:BashLogReceiver: New client";

    if (ret == 1) { // new event
      struct sockaddr_storage their_addr;
      socklen_t len = sizeof (struct sockaddr_storage);
      new_fd = system_->Accept(socket_fd_, (struct sockaddr*) &their_addr, &len);
      if (new_fd < 0) {
        throw exception::detail::LoopAcceptException();
      }

      struct ucred cr;
      socklen_t cr_len = sizeof (struct ucred);
      ret = system_->Getsockopt(new_fd, SOL_SOCKET, SO_PEERCRED, &cr, &cr_len);
      if (ret < 0) {
        throw exception::detail::LoopGetsockoptException();
      }

      ret = system_->Recv(new_fd, command, 199, 0);

      if (ret > 0) {
        command[ret - 1] = '\0';

        time_t tval = system_->Time(nullptr);
        if (tval == ((time_t) (-1))) {
          throw exception::detail::LoopTimeException();
        }

        struct tm *tmval = system_->GMTime(&tval);
        if (tmval == nullptr) {
          throw exception::detail::LoopGMTimeException();
        }

        ret = system_->Gethostname(hostname, 199);
        if (ret < 0) {
          throw exception::detail::LoopGethostnameException();
        }

        BOOST_LOG_TRIVIAL(debug) << "bash:BashLogReceiver: New log entry: "
          << "hostname=" << hostname << " ; "
          << "hour=" << tmval->tm_hour << " ; "
          << "minute=" << tmval->tm_min << " ; "
          << "seconds=" << (tmval->tm_sec % 60) << " ; "
          << "day=" << tmval->tm_mday << " ; "
          << "month=" << (tmval->tm_mon + 1) << " ; "
          << "year=" << (tmval->tm_year + 1900) << " ; "
          << "user_id=" << cr.uid << " ; "
          << "command=" << command;

        type::BashLogEntry log_entry;
        log_entry.hostname = hostname;
        log_entry.utc_time.Set(tmval->tm_hour, tmval->tm_min, tmval->tm_sec % 60,
                               tmval->tm_mday, tmval->tm_mon + 1, tmval->tm_year + 1900);
        log_entry.user_id = cr.uid;
        log_entry.command = command;

        shared_ptr<detail::BashDBusThreadCommand> cmdptr(new detail::BashDBusThreadCommand(log_entry, bash_proxy));
        dbus_thread_->AddCommand(cmdptr);

      } else if (ret == 0) {
        // do nothing
      } else if (ret < 0) {
        throw exception::detail::LoopRecvException();
      }
    } else if (ret == 0) { // timeout
      // do nothing
    } else { // error
      throw exception::detail::LoopPollException();
    }
  }
}

void BashLogReceiver::StopLoop() {
  running_ = false;
  socket_fd_ = -1;
}

bool BashLogReceiver::IsRunning() const {
  return running_;
}

BashLogReceiver::BashLogReceiver(std::shared_ptr<dbus::detail::BusInterface> bus,
                                 std::shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread,
                                 std::shared_ptr<bash::detail::SystemInterface> system)
  : bus_(bus),
  dbus_thread_(dbus_thread),
  system_(system),
  socket_fd_(-1),
  running_(false) {
}

}
