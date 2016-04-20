#include "bash_proxy.h"

#include <boost/log/trivial.hpp>

namespace bash
{

namespace detail
{

BashProxy::BashProxy(std::shared_ptr<::dbus::detail::BusInterface> bus)
  : bus_(bus) {
}

bool BashProxy::AddLogEntry(const type::BashLogEntry &log_entry) {
  BOOST_LOG_TRIVIAL(debug) << "bash:detail:BashProxy:AddLogEntry: Function call";
  BOOST_LOG_TRIVIAL(debug) << "bash:detail:BashProxy:AddLogEntry: Call params: "
    << "hostname=" << log_entry.hostname << " ; "
    << "time=" << log_entry.utc_time << " ; "
    << "user_id=" << log_entry.user_id << " ; "
    << "command=" << log_entry.command;

  DBusMessage *message;
  message = CreateMethodCall("org.chyla.patlms.server",
                             "/org/chyla/patlms/bash",
                             "org.chyla.patlms.bash",
                             "AddLogEntry");

  DBusMessageIter args;
  InitArgument(message, &args);
  AppendArgument(&args, log_entry.hostname.c_str());
  AppendArgument(&args, log_entry.utc_time.GetHour());
  AppendArgument(&args, log_entry.utc_time.GetMinute());
  AppendArgument(&args, log_entry.utc_time.GetSecond());
  AppendArgument(&args, log_entry.utc_time.GetDay());
  AppendArgument(&args, log_entry.utc_time.GetMonth());
  AppendArgument(&args, log_entry.utc_time.GetYear());
  AppendArgument(&args, log_entry.user_id);
  AppendArgument(&args, log_entry.command.c_str());

  DBusPendingCall *reply_handle;
  bus_->SendMessage(message, &reply_handle);

  dbus_message_unref(message);

  dbus_pending_call_block(reply_handle);

  message = GetReplyMessage(reply_handle);

  dbus_pending_call_unref(reply_handle);

  dbus_message_unref(message);
}

}

}
