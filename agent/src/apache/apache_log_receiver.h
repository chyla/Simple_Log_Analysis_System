#ifndef APACHE_LOG_RECEIVER_H
#define APACHE_LOG_RECEIVER_H

#include <patlms/dbus/detail/bus_interface.h>
#include <patlms/network/detail/network_interface.h>
#include <patlms/type/time.h>

#include "src/dbus/detail/dbus_thread_interface.h"

#include <string>
#include <map>

namespace apache
{

class ApacheLogReceiver {
 public:

  static std::shared_ptr<ApacheLogReceiver> Create(std::shared_ptr<dbus::detail::BusInterface> bus,
                                                   std::shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread);

  virtual ~ApacheLogReceiver();

  void OpenSocket(const std::string &socket_path);
  void CloseSocket();

  void StartLoop();
  void StopLoop();

  bool IsRunning() const;
  
  void SetAgentName(const std::string &agent_name);

 private:
  ApacheLogReceiver(std::shared_ptr<dbus::detail::BusInterface> bus,
                    std::shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread,
                    network::detail::NetworkInterfacePtr network);

  type::Time LogTimestampToTime(std::string timestamp);
  
  std::shared_ptr<dbus::detail::BusInterface> bus_;
  std::shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread_;
  network::detail::NetworkInterfacePtr network_;

  int socket_fd_;
  bool running_;
  std::string logbuf;
  const std::map<std::string, int> months;
  std::string agent_name_;
};

}

#endif /* APACHE_LOG_RECEIVER_H */
