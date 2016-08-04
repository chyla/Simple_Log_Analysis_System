#pragma once

#include <patlms/dbus/detail/bus_interface.h>
#include <memory>
#include <list>
#include <mutex>

#include "dbus_thread_command.h"
#include "detail/system.h"
#include "detail/system_interface.h"
#include "detail/dbus_thread_interface.h"

namespace dbus
{

class DBusThread;
typedef std::shared_ptr<DBusThread> DBusThreadPtr;

class DBusThread : public detail::DBusThreadInterface {
 public:
  static DBusThreadPtr Create(detail::BusInterfacePtr bus);
  static DBusThreadPtr Create(detail::BusInterfacePtr bus, detail::SystemInterfacePtr system);

  void AddCommand(DBusThreadCommandPtr command) override;

  void StartLoop() override;
  void StopLoop() override;

  bool IsLoopRunning() override;

 private:
  typedef std::list<DBusThreadCommandPtr> ThreadCommands;

  DBusThread(detail::BusInterfacePtr bus, detail::SystemInterfacePtr system);

  DBusThreadCommandPtr GetCommand();
  bool IsCommandAvailable();

  detail::BusInterfacePtr bus_;
  detail::SystemInterfacePtr system_;

  ThreadCommands commands_;
  std::mutex commands_mutex_;
  bool loop_running_;
};

}
