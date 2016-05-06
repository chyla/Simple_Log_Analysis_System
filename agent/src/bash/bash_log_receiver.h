#ifndef BASH_LOG_RECEIVER_H
#define BASH_LOG_RECEIVER_H



#include <patlms/dbus/detail/bus_interface.h>

#include "detail/system_interface.h"
#include "detail/bash_log_receiver_interface.h"
#include "src/dbus/detail/dbus_thread_interface.h"

namespace bash
{

class BashLogReceiver : public detail::BashLogReceiverInterface {
 public:

  static std::shared_ptr<BashLogReceiver> Create(std::shared_ptr<dbus::detail::BusInterface> bus,
                                                 std::shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread);

  static std::shared_ptr<BashLogReceiver> Create(std::shared_ptr<dbus::detail::BusInterface> bus,
                                                 std::shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread,
                                                 std::shared_ptr<bash::detail::SystemInterface> system);

  virtual ~BashLogReceiver();

  void OpenSocket(const std::string &socket_path) override;

  void StartLoop();
  void StopLoop();

  bool IsRunning() const;
  
  void SetAgentName(const std::string &agent_name);

 private:
  BashLogReceiver(std::shared_ptr<dbus::detail::BusInterface> bus,
                  std::shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread,
                  std::shared_ptr<bash::detail::SystemInterface> system);

  std::shared_ptr<dbus::detail::BusInterface> bus_;
  std::shared_ptr<dbus::detail::DBusThreadInterface> dbus_thread_;
  std::shared_ptr<bash::detail::SystemInterface> system_;

  int socket_fd_;
  bool running_;
  std::string agent_name_;
};

}

#endif /* BASH_LOG_RECEIVER_H */

