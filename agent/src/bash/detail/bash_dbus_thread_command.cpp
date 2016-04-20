#include "bash_dbus_thread_command.h"

namespace bash
{

namespace detail
{

BashDBusThreadCommand::BashDBusThreadCommand(const type::BashLogEntry log_entry,
                                             std::shared_ptr<BashProxy> bash_proxy)
  : log_entry_(log_entry),
  bash_proxy_(bash_proxy) {
}

BashDBusThreadCommand::~BashDBusThreadCommand() {
}

void BashDBusThreadCommand::Execute() {
  bash_proxy_->AddLogEntry(log_entry_);
}

}

}
