#pragma once

#include <memory>

#include <patlms/dbus/detail/bus_interface.h>

#include "system_interface.h"
#include "src/dbus/dbus_thread_command.h"

namespace dbus
{

namespace detail
{

class DBusThreadInterface {
 public:
  virtual ~DBusThreadInterface();

  virtual void AddCommand(dbus::DBusThreadCommandPtr command) = 0;

  virtual void StartLoop() = 0;
  virtual void StopLoop() = 0;

  virtual bool IsLoopRunning() = 0;
};

typedef std::shared_ptr<DBusThreadInterface> DBusThreadInterfacePtr;

}

}
