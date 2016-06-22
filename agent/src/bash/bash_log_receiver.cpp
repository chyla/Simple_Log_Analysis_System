#include "bash_log_receiver.h"

#include <array>
#include <cstring>
#include <boost/log/trivial.hpp>

#include <sys/un.h>

#include <patlms/network/detail/system.h>
#include <patlms/type/bash_log_entry.h>

#include "src/bash/exception/detail/cant_open_socket_exception.h"
#include "src/bash/exception/detail/loop_poll_exception.h"
#include "src/bash/exception/detail/loop_recv_exception.h"
#include "src/bash/exception/detail/loop_accept_exception.h"
#include "src/bash/exception/detail/loop_getsockopt_exception.h"
#include "src/bash/exception/detail/loop_time_exception.h"
#include "src/bash/exception/detail/loop_gmtime_exception.h"
#include "src/bash/exception/detail/socket_is_not_open_exception.h"
#include "detail/bash_dbus_thread_command.h"

using namespace std;
using namespace network;

namespace bash
{

shared_ptr<BashLogReceiver> BashLogReceiver::Create(shared_ptr<dbus::detail::BusInterface> bus,
                                                    shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread) {
  network::detail::NetworkInterfacePtr network = network::Network::Create();
  network::detail::SystemInterfacePtr system(new network::detail::System());
  shared_ptr<BashLogReceiver> receiver(new BashLogReceiver(bus, dbus_thread, network, system));
  return receiver;
}

shared_ptr<BashLogReceiver> BashLogReceiver::Create(shared_ptr<dbus::detail::BusInterface> bus,
                                                    shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread,
                                                    network::detail::NetworkInterfacePtr network,
                                                    network::detail::SystemInterfacePtr system) {
  shared_ptr<BashLogReceiver> receiver(new BashLogReceiver(bus, dbus_thread, network, system));
  return receiver;
}

BashLogReceiver::~BashLogReceiver() {
}

void BashLogReceiver::OpenSocket(const std::string& socket_path) {
  BOOST_LOG_TRIVIAL(debug) << "bash::BashLogReceiver::OpenSocket: Function call with (socket_path=" << socket_path << ")";
  int ret;

  socket_fd_ = network_->OpenUnixSocket(socket_path);

  ret = system_->Chmod(socket_path.c_str(), 0622);
  if (ret < 0) {
    BOOST_LOG_TRIVIAL(warning) << "bash::BashLogReceiver::OpenSocket: Cant change chmod: " << strerror(errno);
  }

  BOOST_LOG_TRIVIAL(debug) << "bash::BashLogReceiver::OpenSocket: Done";
}

void BashLogReceiver::StartLoop() {
  BOOST_LOG_TRIVIAL(debug) << "bash::BashLogReceiver::StartLoop: Function call";

  std::shared_ptr<detail::BashProxy> bash_proxy(new detail::BashProxy(bus_));
  WaitStatus ws;
  ConnectionData connection_data;
  string new_command;

  running_ = true;
  while (running_) {
    ws = network_->WaitForData(socket_fd_, 5);

    if (ws == WaitStatus::NEW_DATA) {
      BOOST_LOG_TRIVIAL(debug) << "bash::BashLogReceiver::StartLoop: New data";

      connection_data = network_->Accept(socket_fd_);
      new_command = network_->ReceiveText(connection_data.socket);

      struct ucred cr;
      socklen_t cr_len = sizeof (struct ucred);
      int ret = system_->Getsockopt(connection_data.socket, SOL_SOCKET, SO_PEERCRED, &cr, &cr_len);
      if (ret < 0) {
        throw exception::detail::LoopGetsockoptException();
      }

      network_->Close(connection_data.socket);

      time_t tval = system_->Time(nullptr);
      if (tval == ((time_t) (-1))) {
        throw exception::detail::LoopTimeException();
      }

      struct tm *tmval = system_->GMTime(&tval);
      if (tmval == nullptr) {
        throw exception::detail::LoopGMTimeException();
      }

      BOOST_LOG_TRIVIAL(debug) << "bash::BashLogReceiver::StartLoop: New log entry: "
          << "agent_name=" << agent_name_ << " ; "
          << "hour=" << tmval->tm_hour << " ; "
          << "minute=" << tmval->tm_min << " ; "
          << "seconds=" << (tmval->tm_sec % 60) << " ; "
          << "day=" << tmval->tm_mday << " ; "
          << "month=" << (tmval->tm_mon + 1) << " ; "
          << "year=" << (tmval->tm_year + 1900) << " ; "
          << "user_id=" << cr.uid << " ; "
          << "command=" << new_command;

      type::BashLogEntry log_entry;
      log_entry.agent_name = agent_name_;
      log_entry.utc_time.Set(tmval->tm_hour, tmval->tm_min, tmval->tm_sec % 60,
                             tmval->tm_mday, tmval->tm_mon + 1, tmval->tm_year + 1900);
      log_entry.user_id = cr.uid;
      log_entry.command = new_command;

      shared_ptr<detail::BashDBusThreadCommand> cmdptr(new detail::BashDBusThreadCommand(log_entry, bash_proxy));
      dbus_thread_->AddCommand(cmdptr);
    }
    else if (ws == WaitStatus::TIMEOUT) {
      BOOST_LOG_TRIVIAL(debug) << "bash::BashLogReceiver::StartLoop: Timeout";
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

void BashLogReceiver::SetAgentName(const std::string &agent_name) {
  agent_name_ = agent_name;
}

BashLogReceiver::BashLogReceiver(std::shared_ptr<dbus::detail::BusInterface> bus,
                                 std::shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread,
                                 network::detail::NetworkInterfacePtr network,
                                 ::network::detail::SystemInterfacePtr system)
: bus_(bus),
dbus_thread_(dbus_thread),
network_(network),
system_(system),
socket_fd_(-1),
running_(false) {
}

}
