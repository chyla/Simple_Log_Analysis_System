#ifndef PATLMS_DBUS_DBUS_THREAD_COMMAND_H
#define PATLMS_DBUS_DBUS_THREAD_COMMAND_H

#include <memory>

namespace dbus
{

class DBusThreadCommand
{
 public:
  virtual ~DBusThreadCommand();
  
  virtual void Execute() = 0;
};

typedef std::shared_ptr<DBusThreadCommand> DBusThreadCommandPtr;

}

#endif /* PATLMS_DBUS_DBUS_THREAD_COMMAND_H */

