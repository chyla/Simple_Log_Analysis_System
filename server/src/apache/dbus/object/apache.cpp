#include "apache.h"

#include <boost/log/trivial.hpp>

namespace apache
{

namespace dbus
{

namespace object
{

Apache::Apache(::database::DatabasePtr database,
               ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
               ::apache::database::detail::DatabaseFunctionsInterfacePtr apache_database_functions) :
database_(database),
general_database_functions_(general_database_functions),
apache_database_functions_(apache_database_functions) {
}

Apache::~Apache() {
}

const char* Apache::GetPath() {
  return "/org/chyla/patlms/apache";
}

const char* Apache::GetXmlInterface() {
  const char *xml =
      "<!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\"\n"
      "\"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">\n"
      "<node>\n"
      "  <interface name=\"org.freedesktop.DBus.Introspectable\">\n"
      "    <method name=\"Introspect\">\n"
      "      <arg name=\"data\" direction=\"out\" type=\"s\"/>\n"
      "    </method>\n"
      "  </interface>\n"
      "  <interface name=\"org.chyla.patlms.apache\">\n"
      "    <method name=\"AddLogEntry\">\n"
      "      <arg direction=\"in\" type=\"s\"/>\n"
      "      <arg direction=\"in\" type=\"s\"/>\n"
      "      <arg direction=\"in\" type=\"s\"/>\n"
      "      <arg direction=\"in\" type=\"i\"/>\n"
      "      <arg direction=\"in\" type=\"i\"/>\n"
      "      <arg direction=\"in\" type=\"i\"/>\n"
      "      <arg direction=\"in\" type=\"i\"/>\n"
      "      <arg direction=\"in\" type=\"i\"/>\n"
      "      <arg direction=\"in\" type=\"i\"/>\n"
      "      <arg direction=\"in\" type=\"s\"/>\n"
      "      <arg direction=\"in\" type=\"i\"/>\n"
      "      <arg direction=\"in\" type=\"i\"/>\n"
      "      <arg direction=\"in\" type=\"s\"/>\n"
      "      <arg direction=\"out\" type=\"v\"/>\n"
      "    </method>\n"
      "  </interface>\n"
      "</node>\n";

  return xml;
}

DBusHandlerResult Apache::OwnMessageHandler(DBusConnection *connection, DBusMessage *message) {
  BOOST_LOG_TRIVIAL(debug) << "objects::Apache::OwnMessageHandler: Function call";

  if (dbus_message_is_method_call(message, "org.chyla.patlms.apache", "AddLogEntry")) {
    BOOST_LOG_TRIVIAL(debug) << "objects:Apache:OwnMessageHandler: Received method call org.chyla.patlms.apache.AddLogEntry";

    const char *agent_name = nullptr;
    const char *virtualhost = nullptr;
    const char *client_ip = nullptr;
    int hour, minute, second, day, month, year;
    const char* request = nullptr;
    int status_code, bytes;
    const char *user_agent = nullptr;
    dbus_message_get_args(message, NULL,
                          DBUS_TYPE_STRING, &agent_name,
                          DBUS_TYPE_STRING, &virtualhost,
                          DBUS_TYPE_STRING, &client_ip,
                          DBUS_TYPE_INT32, &hour,
                          DBUS_TYPE_INT32, &minute,
                          DBUS_TYPE_INT32, &second,
                          DBUS_TYPE_INT32, &day,
                          DBUS_TYPE_INT32, &month,
                          DBUS_TYPE_INT32, &year,
                          DBUS_TYPE_STRING, &request,
                          DBUS_TYPE_INT32, &status_code,
                          DBUS_TYPE_INT32, &bytes,
                          DBUS_TYPE_STRING, &user_agent,
                          DBUS_TYPE_INVALID);

    BOOST_LOG_TRIVIAL(debug) << "objects::Apache::OwnMessageHandler: Add log entry to cache: "
        << "agent_name=" << agent_name << " ; "
        << "virtualhost=" << virtualhost << " ; "
        << "client_ip=" << client_ip << " ; "
        << "hour=" << hour << " ; "
        << "minute=" << minute << " ; "
        << "second=" << second << " ; "
        << "day=" << day << " ; "
        << "month=" << month << " ; "
        << "year=" << year << " ; "
        << "request=" << request << " ; "
        << "bytes=" << bytes << " ; "
        << "user_agent=" << user_agent;

    ::type::ApacheLogEntry log_entry;
    log_entry.agent_name = agent_name;
    log_entry.virtualhost = virtualhost;
    log_entry.client_ip = client_ip;
    log_entry.time.Set(hour, minute, second, day, month, year);
    log_entry.request = request;
    log_entry.status_code = status_code;
    log_entry.bytes = bytes;
    log_entry.user_agent = user_agent;

    general_database_functions_->AddAgentName(agent_name);
    apache_database_functions_->AddVirtualhostName(virtualhost);

    apache_database_functions_->AddLogs({log_entry});

    DBusMessage *reply_msg = dbus_message_new_method_return(message);
    BOOST_LOG_TRIVIAL(debug) << "objects::Apache::OwnMessageHandler: Sending reply";
    dbus_connection_send(connection, reply_msg, NULL);

    BOOST_LOG_TRIVIAL(debug) << "objects::Apache::OwnMessageHandler: Connection flushing";
    dbus_connection_flush(connection);

    BOOST_LOG_TRIVIAL(debug) << "objects::Apache::OwnMessageHandler: Done. Returning DBUS_HANDLER_RESULT_HANDLED";

    return DBUS_HANDLER_RESULT_HANDLED;
  }

  BOOST_LOG_TRIVIAL(warning) << "objects::Apache::OwnMessageHandler: Possible bug: DBUS_HANDLER_RESULT_NOT_YET_HANDLED";

  return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

}

}

}
