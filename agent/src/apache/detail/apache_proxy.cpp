#include "apache_proxy.h"

#include <boost/log/trivial.hpp>

namespace apache
{

namespace detail
{

ApacheProxy::ApacheProxy(std::shared_ptr<::dbus::detail::BusInterface> bus)
  : bus_(bus) {
}

bool ApacheProxy::AddLogEntry(const type::ApacheLogEntry &log_entry) {
  BOOST_LOG_TRIVIAL(debug) << "apache:detail:ApacheProxy:AddLogEntry: Function call";

  DBusMessage *message;
  message = CreateMethodCall("org.chyla.patlms.server",
                             "/org/chyla/patlms/apache",
                             "org.chyla.patlms.apache",
                             "AddLogEntry");

  DBusMessageIter args;
  InitArgument(message, &args);
  AppendArgument(&args, log_entry.virtualhost.c_str());
  AppendArgument(&args, log_entry.client_ip.c_str());
  AppendArgument(&args, log_entry.time.GetHour());
  AppendArgument(&args, log_entry.time.GetMinute());
  AppendArgument(&args, log_entry.time.GetSecond());
  AppendArgument(&args, log_entry.time.GetDay());
  AppendArgument(&args, log_entry.time.GetMonth());
  AppendArgument(&args, log_entry.time.GetYear());
  AppendArgument(&args, log_entry.request.c_str());
  AppendArgument(&args, log_entry.status_code);
  AppendArgument(&args, log_entry.bytes);
  AppendArgument(&args, log_entry.user_agent.c_str());

  
  
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
