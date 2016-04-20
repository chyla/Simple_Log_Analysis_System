#include <patlms/dbus/proxy_object.h>

#include <boost/log/trivial.hpp>


namespace dbus
{

ProxyObject::ProxyObject() {
}

ProxyObject::~ProxyObject() {
}

DBusMessage* ProxyObject::CreateMethodCall(const char *bus_name,
                                           const char *object_name,
                                           const char *interface_name,
                                           const char *method_name) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::ProxyObject::CreateMethodCall: Funxtion call";

  DBusMessage *message;
  message = dbus_message_new_method_call(bus_name,
                                         object_name,
                                         interface_name,
                                         method_name);

  if (message == nullptr)
    BOOST_LOG_TRIVIAL(error) << "dbus::ProxyObject::CreateMethodCall: Failed to create message: out of memory";

  return message;
}

void ProxyObject::InitArgument(DBusMessage *message, DBusMessageIter *args) {
  dbus_message_iter_init_append(message, args);
}

bool ProxyObject::AppendArgument(DBusMessageIter *iter_args,
                                 const char *param) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::ProxyObject::AppendArgument(const char*): Function call";

  bool ret = dbus_message_iter_append_basic(iter_args, DBUS_TYPE_STRING, &param);

  if (!ret)
    BOOST_LOG_TRIVIAL(error) << "dbus::ProxyObject::AppendArgument: Failed to append argument: out of memory";

  return ret;
}

bool ProxyObject::AppendArgument(DBusMessageIter *iter_args, int param) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::ProxyObject::AppendArgument(int): Function call";

  bool ret = dbus_message_iter_append_basic(iter_args, DBUS_TYPE_INT32, &param);

  if (!ret)
    BOOST_LOG_TRIVIAL(error) << "dbus::ProxyObject::AppendArgument: Failed to append argument: out of memory";

  return ret;
}

bool ProxyObject::AppendArgument(DBusMessageIter *iter_args, unsigned param) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::ProxyObject::AppendArgument(unsigned): Function call";

  bool ret = dbus_message_iter_append_basic(iter_args, DBUS_TYPE_UINT32, &param);

  if (!ret)
    BOOST_LOG_TRIVIAL(error) << "dbus::ProxyObject::AppendArgument: Failed to append argument: out of memory";

  return ret;
}

DBusMessage* ProxyObject::GetReplyMessage(DBusPendingCall *reply_handle) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::ProxyObject::GetReplyMessage: Function call";

  DBusMessage *message = dbus_pending_call_steal_reply(reply_handle);

  if (message == nullptr)
    BOOST_LOG_TRIVIAL(error) << "dbus::ProxyObject::AppendArgument: Failed to steal reply";

  return message;
}

void ProxyObject::FreePendingCall(DBusPendingCall *reply_handle) {
  dbus_pending_call_unref(reply_handle);
}

}
