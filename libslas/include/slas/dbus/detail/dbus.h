/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "dbus_interface.h"

namespace dbus
{

namespace detail
{

class DBus;
typedef std::shared_ptr<DBus> DBusPtr;

class DBus : public DBusInterface {
 public:
  virtual ~DBus() = default;

  static DBusPtr Create();

  dbus_bool_t bus_register(DBusConnection *connection,
                           DBusError *error) override;

  DBusConnection* connection_open_private(const char *address,
                                          DBusError *error) override;

  void connection_set_exit_on_disconnect(DBusConnection *connection,
                                         dbus_bool_t exit_on_disconnect) override;

  void connection_close(DBusConnection *connection) override;

  int bus_request_name(DBusConnection *connection,
                       const char *name,
                       unsigned int flags,
                       DBusError *error) override;

  dbus_bool_t error_is_set(const DBusError *error) override;

  dbus_bool_t connection_register_object_path(DBusConnection *connection,
                                              const char *path,
                                              const DBusObjectPathVTable *vtable,
                                              void *user_data) override;

  dbus_bool_t connection_read_write(DBusConnection *connection,
                                    int timeout_milliseconds) override;

  DBusMessage* connection_borrow_message(DBusConnection *connection) override;

  void connection_return_message(DBusConnection *connection,
                                 DBusMessage *message) override;

  DBusDispatchStatus connection_dispatch(DBusConnection *connection) override;

  dbus_bool_t connection_send_with_reply(DBusConnection *connection,
                                         DBusMessage *message,
                                         DBusPendingCall **pending_return,
                                         int timeout_milliseconds) override;

  void connection_flush(DBusConnection *connection) override;

  dbus_bool_t connection_unregister_object_path(DBusConnection *connection,
                                                const char *path) override;

  void connection_unref(DBusConnection *connection) override;

  void error_init(DBusError *error) override;

  void error_free(DBusError *error) override;
};

}

}
