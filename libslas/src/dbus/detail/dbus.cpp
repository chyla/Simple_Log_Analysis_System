/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <slas/dbus/detail/dbus.h>

#include <boost/log/trivial.hpp>

namespace dbus
{

namespace detail
{

DBusPtr DBus::Create() {
  return std::make_shared<DBus>();
}

dbus_bool_t DBus::bus_register(DBusConnection *connection,
                               DBusError *error) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::dbus_bus_register: Function call";
  return dbus_bus_register(connection, error);
}

DBusConnection* DBus::connection_open_private(const char *address,
                                              DBusError *error) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::dbus_connection_open_private: Function call";
  return dbus_connection_open_private(address, error);
}

void DBus::connection_set_exit_on_disconnect(DBusConnection *connection,
                                             dbus_bool_t exit_on_disconnect) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::dbus_connection_set_exit_on_disconnect: Function call";
  return dbus_connection_set_exit_on_disconnect(connection, exit_on_disconnect);
}

void DBus::connection_close(DBusConnection *connection) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::dbus_connection_close: Function call";
  return dbus_connection_close(connection);
}

int DBus::bus_request_name(DBusConnection *connection,
                           const char *name,
                           unsigned int flags,
                           DBusError *error) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::dbus_bus_request_name: Function call";
  return dbus_bus_request_name(connection, name, flags, error);
}

dbus_bool_t DBus::error_is_set(const DBusError *error) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::dbus_error_is_set: Function call";
  return dbus_error_is_set(error);
}

dbus_bool_t DBus::connection_register_object_path(DBusConnection *connection,
                                                  const char *path,
                                                  const DBusObjectPathVTable *vtable,
                                                  void *user_data) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::dbus_connection_register_object_path: Function call";
  return dbus_connection_register_object_path(connection, path, vtable, user_data);
}

dbus_bool_t DBus::connection_read_write(DBusConnection *connection,
                                        int timeout_milliseconds) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::dbus_connection_read_write: Function call";
  return dbus_connection_read_write(connection, timeout_milliseconds);
}

DBusMessage* DBus::connection_borrow_message(DBusConnection *connection) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::dbus_connection_borrow_message: Function call";
  return dbus_connection_borrow_message(connection);
}

void DBus::connection_return_message(DBusConnection *connection,
                                     DBusMessage *message) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::dbus_connection_return_message: Function call";
  return dbus_connection_return_message(connection, message);
}

DBusDispatchStatus DBus::connection_dispatch(DBusConnection *connection) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::dbus_connection_dispatch: Function call";
  return dbus_connection_dispatch(connection);
}

dbus_bool_t DBus::connection_send_with_reply(DBusConnection *connection,
                                             DBusMessage *message,
                                             DBusPendingCall **pending_return,
                                             int timeout_milliseconds) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::dbus_connection_send_with_reply: Function call";
  return dbus_connection_send_with_reply(connection, message, pending_return, timeout_milliseconds);
}

void DBus::connection_flush(DBusConnection *connection) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::dbus_connection_flush: Function call";
  return dbus_connection_flush(connection);
}

dbus_bool_t DBus::connection_unregister_object_path(DBusConnection *connection,
                                                    const char *path) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::dbus_connection_unregister_object_path: Function call";
  return dbus_connection_unregister_object_path(connection, path);
}

void DBus::connection_unref(DBusConnection *connection) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::connection_unref: Function call";

  return dbus_connection_unref(connection);
}

void DBus::error_init(DBusError *error) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::error_init: Function call";
  dbus_error_init(error);
}

void DBus::error_free(DBusError *error) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBus::error_free: Function call";
  dbus_error_free(error);
}

}

}
