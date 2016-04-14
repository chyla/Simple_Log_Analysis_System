#include "object.h"

#include <cstring>
#include <boost/log/trivial.hpp>


namespace dbus
{

Object::~Object()
{
}
  
DBusHandlerResult Object::MessageHandler(DBusConnection *connection, DBusMessage *message)
{
  BOOST_LOG_TRIVIAL(debug) << "dbus::Object::MessageHandler: Function call";

  const char *interface = dbus_message_get_interface(message);
  BOOST_LOG_TRIVIAL(debug) << "dbus::Object::MessageHandler: Interface: " << interface;
  
  if (strcmp(interface, DBUS_INTERFACE_INTROSPECTABLE) == 0)
  {
    BOOST_LOG_TRIVIAL(debug) << "dbus::Object::MessageHandler: Introspection function called";

    const char *xml = GetXmlInterface();
    BOOST_LOG_TRIVIAL(debug) << "dbus::Object::MessageHandler: XML introspection: " << xml;
    
    DBusMessage *reply_msg = dbus_message_new_method_return(message);

    DBusMessageIter args;
    dbus_message_iter_init_append(reply_msg, &args);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &xml);

    BOOST_LOG_TRIVIAL(debug) << "dbus::Object::MessageHandler: Sending reply";
    dbus_connection_send(connection, reply_msg, NULL);

    BOOST_LOG_TRIVIAL(debug) << "dbus::Object::MessageHandler: Connection flushing";
    dbus_connection_flush(connection);

    BOOST_LOG_TRIVIAL(debug) << "dbus::Object::MessageHandler: Done. Returning DBUS_HANDLER_RESULT_HANDLED";

    return DBUS_HANDLER_RESULT_HANDLED;
  }

  return OwnMessageHandler(connection, message);
}

}
