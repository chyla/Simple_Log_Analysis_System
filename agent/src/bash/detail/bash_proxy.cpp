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
    << "agent_name=" << log_entry.agent_name << " ; "
    << "time=" << log_entry.utc_time << " ; "
    << "user_id=" << log_entry.user_id << " ; "
    << "command=" << log_entry.command;

  DBusMessage *message;
  message = CreateMethodCall("org.chyla.slas.server",
                             "/org/chyla/slas/bash",
                             "org.chyla.slas.bash",
                             "AddLogEntry");

  DBusMessageIter args;
  InitArgument(message, &args);
  AppendArgument(&args, log_entry.agent_name.c_str());
  AppendArgument(&args, log_entry.utc_time.GetTime().GetHour());
  AppendArgument(&args, log_entry.utc_time.GetTime().GetMinute());
  AppendArgument(&args, log_entry.utc_time.GetTime().GetSecond());
  AppendArgument(&args, log_entry.utc_time.GetDate().GetDay());
  AppendArgument(&args, log_entry.utc_time.GetDate().GetMonth());
  AppendArgument(&args, log_entry.utc_time.GetDate().GetYear());
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
