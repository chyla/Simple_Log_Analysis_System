#ifndef PATLMS_DBUS_DETAIL_DBUS_THREAD_INTERFACE_H
#define PATLMS_DBUS_DETAIL_DBUS_THREAD_INTERFACE_H

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
  typedef std::shared_ptr<::dbus::DBusThreadCommand> CommandPtr;
  
  virtual ~DBusThreadInterface();

  virtual void AddCommand(CommandPtr command) = 0;

  virtual void StartLoop() = 0;
  virtual void StopLoop() = 0;

  virtual bool IsLoopRunning() = 0;
};

}

}


#endif /* PATLMS_DBUS_DETAIL_DBUS_THREAD_INTERFACE_H */

