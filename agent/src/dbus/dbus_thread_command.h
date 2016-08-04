#pragma once

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
