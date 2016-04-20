#ifndef PATLMS_BASH_DETAIL_BASH_DBUS_THREAD_COMMAND_H
#define PATLMS_BASH_DETAIL_BASH_DBUS_THREAD_COMMAND_H

#include "src/dbus/dbus_thread_command.h"

#include <string>

#include <patlms/type/bash_log_entry.h>

#include "bash_proxy.h"

namespace bash
{

namespace detail
{

class BashDBusThreadCommand : public ::dbus::DBusThreadCommand {
 public:
  BashDBusThreadCommand(const type::BashLogEntry log_entry,
                        std::shared_ptr<BashProxy> bash_proxy);
  virtual ~BashDBusThreadCommand();

  void Execute() override;

 private:
  const type::BashLogEntry log_entry_;
  std::shared_ptr<BashProxy> bash_proxy_;
};

}

}

#endif /* PATLMS_BASH_DETAIL_BASH_DBUS_THREAD_COMMAND_H */
