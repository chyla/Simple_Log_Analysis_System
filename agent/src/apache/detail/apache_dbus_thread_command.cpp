#include "apache_dbus_thread_command.h"

namespace apache
{

namespace detail
{

ApacheDBusThreadCommand::ApacheDBusThreadCommand(const type::ApacheLogEntry log_entry,
                                                 std::shared_ptr<ApacheProxy> proxy)
  : log_entry_(log_entry),
  proxy_(proxy) {
}

ApacheDBusThreadCommand::~ApacheDBusThreadCommand() {
}

void ApacheDBusThreadCommand::Execute() {
  proxy_->AddLogEntry(log_entry_);
}

}

}
