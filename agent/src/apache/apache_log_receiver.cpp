#include "apache_log_receiver.h"

#include <array>
#include <cstring>
#include <ctime>
#include <regex>
#include <string>
#include <boost/log/trivial.hpp>

#include <patlms/network/network.h>
#include <patlms/network/exception/detail/recv_exception.h>
#include <patlms/network/exception/detail/timeout_exception.h>

#include "detail/apache_dbus_thread_command.h"
#include "detail/apache_proxy.h"

using namespace std;
using namespace network;

namespace apache
{

shared_ptr<ApacheLogReceiver> ApacheLogReceiver::Create(shared_ptr<dbus::detail::BusInterface> bus,
                                                        shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread) {
  NetworkPtr network = Network::Create();
  shared_ptr<ApacheLogReceiver> receiver(new ApacheLogReceiver(bus, dbus_thread, network));
  return receiver;
}

ApacheLogReceiver::~ApacheLogReceiver() {
}

void ApacheLogReceiver::OpenSocket(const std::string& socket_path) {
  int ret;

  ret = unlink(socket_path.c_str());
  if (ret < 0) {
    BOOST_LOG_TRIVIAL(warning) << "apache::ApacheLogReceiver::OpenSocket: Cant unlink file: " << strerror(errno);
  }

  socket_fd_ = network_->OpenUnixSocket(socket_path);

  ret = chmod(socket_path.c_str(), 0622);
  if (ret < 0) {
    BOOST_LOG_TRIVIAL(warning) << "apache::ApacheLogReceiver::OpenSocket: Cant change chmod: " << strerror(errno);
  }
}

void ApacheLogReceiver::StartLoop() {
  std::shared_ptr<detail::ApacheProxy> proxy(new detail::ApacheProxy(bus_));
  WaitStatus ws;
  ConnectionData connection_data;
  string new_log;
  regex pattern("<([^>]*)> <([^>]*)> <([^>]*)> <([^>]*)> <([^>]*)> <([^>]*)> <([^>]*)>");

  running_ = true;
  while (running_) {
    ws = network_->WaitForData(socket_fd_, 5);

    if (ws == WaitStatus::NEW_DATA) {
      BOOST_LOG_TRIVIAL(debug) << "apache::ApacheLogReceiver::StartLoop: New client";

      connection_data = network_->Accept(socket_fd_);

      new_log = network_->ReceiveText(connection_data.socket);

      smatch match;
      if (regex_match(new_log, match, pattern)) {
        BOOST_LOG_TRIVIAL(debug) << "apache::ApacheLogReceiver::LogTimestampToTime: New log line match regex";

        type::ApacheLogEntry log_entry;
        log_entry.virtualhost = match[1];
        log_entry.client_ip = match[2];
        log_entry.time = LogTimestampToTime(match[3]);
        log_entry.request = match[4];
        log_entry.status_code = stoi(match[5]);
        log_entry.bytes = stoi(match[6]);
        log_entry.user_agent = match[7];

        shared_ptr<detail::ApacheDBusThreadCommand> cmdptr(new detail::ApacheDBusThreadCommand(log_entry, proxy));
        dbus_thread_->AddCommand(cmdptr);
      }
      else {
        BOOST_LOG_TRIVIAL(warning) << "apache::ApacheLogReceiver::StartLoop: Failed to match log line to rexex, broken log line";
      }

      network_->Close(connection_data.socket);
    }
    else if (ws == WaitStatus::TIMEOUT) {
      BOOST_LOG_TRIVIAL(debug) << "apache::ApacheLogReceiver::StartLoop: Timeout";
    }
  }
}

void ApacheLogReceiver::StopLoop() {

  running_ = false;
  socket_fd_ = -1;
}

bool ApacheLogReceiver::IsRunning() const {

  return running_;
}

ApacheLogReceiver::ApacheLogReceiver(std::shared_ptr<dbus::detail::BusInterface> bus,
                                     std::shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread,
                                     network::detail::NetworkInterfacePtr network)
: bus_(bus),
dbus_thread_(dbus_thread),
network_(network),
socket_fd_(-1),
running_(false),
months{
  {"Jan", 1},
  {"Feb", 2},
  {"Mar", 3},
  {"Apr", 4},
  {"May", 5},
  {"Jun", 6},
  {"Jul", 7},
  {"Aug", 8},
  {"Sep", 9},
  {"Oct", 10},
  {"Nov", 11},
  {"Dec", 12}
}
{
}

type::Time ApacheLogReceiver::LogTimestampToTime(string timestamp) {
  BOOST_LOG_TRIVIAL(debug) << "apache::ApacheLogReceiver::LogTimestampToTime: Function call with (timestamp=" << timestamp << ")";
  type::Time time;
  regex pattern("^\\[(\\d*)/([A-Za-z]*)/(\\d*):(\\d*):(\\d*):(\\d*).*");
  smatch match;
  int d, mo, y, h, m, s;

  if (regex_match(timestamp, match, pattern)) {
    BOOST_LOG_TRIVIAL(debug) << "apache::ApacheLogReceiver::LogTimestampToTime: Timestamp match regex";

    d = stoi(match[1]);
    mo = months.at(match[2]);
    y = stoi(match[3]);
    h = stoi(match[4]);
    m = stoi(match[5]);
    s = stoi(match[6]);
    
    BOOST_LOG_TRIVIAL(debug) << "apache::ApacheLogReceiver::LogTimestampToTime: Parsed: day=" << d << "; month=" << mo << "; year=" << y << "; hour=" << h << "; minute=" << m << "; second=" << s;

    time.Set(h, m, s, d, mo, y);
  }
  else {
    BOOST_LOG_TRIVIAL(error) << "apache::ApacheLogReceiver::LogTimestampToTime: Failed to match timestamp to regex";
  }

  return time;
}

}
