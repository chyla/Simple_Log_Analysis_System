#include "bash.h"

#include <boost/log/trivial.hpp>


namespace objects
{

Bash::~Bash()
{
}

const char* Bash::GetPath()
{
  return "/org/chyla/patlms/bash";
}

const char* Bash::GetXmlInterface()
{
  const char *xml =
    "<!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\"\n"
    "\"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">\n"
    "<node>\n"
    "  <interface name=\"org.freedesktop.DBus.Introspectable\">\n"
    "    <method name=\"Introspect\">\n"
    "      <arg name=\"data\" direction=\"out\" type=\"s\"/>\n"
    "    </method>\n"
    "  </interface>\n"
    "  <interface name=\"org.chyla.patlms.bash\">\n"
    "    <method name=\"AddLogEntry\">\n"
    "      <arg direction=\"in\" type=\"s\"/>\n"
    "      <arg direction=\"out\" type=\"v\"/>\n"
    "    </method>\n"
    "  </interface>\n"
    "</node>\n";

  return xml;
}

DBusHandlerResult Bash::OwnMessageHandler(DBusConnection *connection, DBusMessage *message)
{
  BOOST_LOG_TRIVIAL(debug) << "objects::Bash::OwnMessageHandler: Function call";

  if (dbus_message_is_method_call(message, "org.chyla.patlms.bash", "AddLogEntry"))
  {
    BOOST_LOG_TRIVIAL(debug) << "objects:Bash:OwnMessageHandler: Received method call org.chyla.patlms.bash.AddLogEntry";

    const char* log_entry = nullptr;
    dbus_message_get_args(message, NULL, DBUS_TYPE_STRING, &log_entry, DBUS_TYPE_INVALID);

    BOOST_LOG_TRIVIAL(debug) << "objects::Bash::OwnMessageHandler: Add log entry: " << (log_entry ? log_entry : "<nullptr>");

    DBusMessage *reply_msg = dbus_message_new_method_return(message);
    BOOST_LOG_TRIVIAL(debug) << "objects::Bash::OwnMessageHandler: Sending reply";
    dbus_connection_send(connection, reply_msg, NULL);

    BOOST_LOG_TRIVIAL(debug) << "objects::Bash::OwnMessageHandler: Connection flushing";
    dbus_connection_flush(connection);

    BOOST_LOG_TRIVIAL(debug) << "objects::Bash::OwnMessageHandler: Done. Returning DBUS_HANDLER_RESULT_HANDLED";

    return DBUS_HANDLER_RESULT_HANDLED;
  }

  BOOST_LOG_TRIVIAL(warning) << "objects::Bash::OwnMessageHandler: Possible bug: DBUS_HANDLER_RESULT_NOT_YET_HANDLED";

  return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}
  
}
