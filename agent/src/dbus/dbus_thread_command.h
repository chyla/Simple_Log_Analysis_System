#ifndef PATLMS_DBUS_DBUS_THREAD_COMMAND_H
#define PATLMS_DBUS_DBUS_THREAD_COMMAND_H

namespace dbus
{

class DBusThreadCommand
{
 public:
  virtual ~DBusThreadCommand();
  
  virtual void Execute() = 0;
};

}

#endif /* PATLMS_DBUS_DBUS_THREAD_COMMAND_H */

