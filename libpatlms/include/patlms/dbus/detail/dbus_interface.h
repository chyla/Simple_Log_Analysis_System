/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBPATLMS_DBUS_DETAIL_SQLITE_INTERFACE_H
#define LIBPATLMS_DBUS_DETAIL_SQLITE_INTERFACE_H

#include <memory>
#include <dbus/dbus.h>

namespace dbus
{

namespace detail
{

class DBusInterface {
 public:
  virtual ~DBusInterface() = default;

  virtual dbus_bool_t bus_register(DBusConnection *connection,
                                   DBusError *error) = 0;

  virtual DBusConnection* connection_open_private(const char *address,
                                                  DBusError *error) = 0;

  virtual void connection_set_exit_on_disconnect(DBusConnection *connection,
                                                 dbus_bool_t exit_on_disconnect) = 0;

  virtual void connection_close(DBusConnection *connection) = 0;

  virtual int bus_request_name(DBusConnection *connection,
                               const char *name,
                               unsigned int flags,
                               DBusError *error) = 0;

  virtual dbus_bool_t error_is_set(const DBusError *error) = 0;

  virtual dbus_bool_t connection_register_object_path(DBusConnection *connection,
                                                      const char *path,
                                                      const DBusObjectPathVTable *vtable,
                                                      void *user_data) = 0;

  virtual dbus_bool_t connection_read_write(DBusConnection *connection,
                                            int timeout_milliseconds) = 0;

  virtual DBusMessage* connection_borrow_message(DBusConnection *connection) = 0;

  virtual void connection_return_message(DBusConnection *connection,
                                         DBusMessage *message) = 0;

  virtual DBusDispatchStatus connection_dispatch(DBusConnection *connection) = 0;

  virtual dbus_bool_t connection_send_with_reply(DBusConnection *connection,
                                                 DBusMessage *message,
                                                 DBusPendingCall **pending_return,
                                                 int timeout_milliseconds) = 0;

  virtual void connection_flush(DBusConnection *connection) = 0;

  virtual dbus_bool_t connection_unregister_object_path(DBusConnection *connection,
                                                        const char *path) = 0;
};

typedef std::shared_ptr<DBusInterface> DBusInterfacePtr;

}

}

#endif /* SQLITE_INTERFACE_H */
