#include "bash_proxy.h"

namespace objects
{

BashProxy::BashProxy(dbus::Bus &bus)
  : bus_(bus)
{
}

bool BashProxy::AddLogEntry(const std::string &log_entry)
{
  DBusMessage *message;
  message = CreateMethodCall("org.chyla.patlms.server",
                             "/org/chyla/patlms/bash",
                             "org.chyla.patlms.bash",
                             "AddLogEntry");

  DBusMessageIter args;
  InitArgument(message, &args);
  AppendArgument(&args, log_entry.c_str());

  DBusPendingCall *reply_handle;
  bus_.SendMessage(message, &reply_handle);

  dbus_message_unref(message);

  dbus_pending_call_block(reply_handle);

  message = GetReplyMessage(reply_handle);

  dbus_pending_call_unref(reply_handle);

  dbus_message_unref(message);
}

}
