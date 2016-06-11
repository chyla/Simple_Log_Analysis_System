/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LIBPATLMS_TESTS_MOCK_DBUS_DETAIL_DBUS_H
#define LIBPATLMS_TESTS_MOCK_DBUS_DETAIL_DBUS_H

#include <dbus/dbus.h>
#include <gmock/gmock.h>
#include <memory>

#include <patlms/dbus/detail/dbus_interface.h>

namespace mock
{

namespace dbus
{

namespace detail
{

class DBus;
typedef std::shared_ptr<DBus> DBusPtr;

class DBus : public ::dbus::detail::DBusInterface {
 public:
  virtual ~DBus() = default;

  static DBusPtr Create() {
    return std::make_shared<DBus>();
  }

  MOCK_METHOD2(bus_register, dbus_bool_t(DBusConnection *connection,
                                         DBusError *error));

  MOCK_METHOD2(connection_open_private, DBusConnection*(const char *address,
               DBusError *error));

  MOCK_METHOD2(connection_set_exit_on_disconnect, void(DBusConnection *connection,
               dbus_bool_t exit_on_disconnect));

  MOCK_METHOD1(connection_close, void(DBusConnection *connection));

  MOCK_METHOD4(bus_request_name, int(DBusConnection *connection,
               const char *name,
               unsigned int flags,
               DBusError *error));

  MOCK_METHOD1(error_is_set, dbus_bool_t(const DBusError *error));

  MOCK_METHOD4(connection_register_object_path, dbus_bool_t(DBusConnection *connection,
                                                            const char *path,
                                                            const DBusObjectPathVTable *vtable,
                                                            void *user_data));

  MOCK_METHOD2(connection_read_write, dbus_bool_t(DBusConnection *connection,
                                                  int timeout_milliseconds));

  MOCK_METHOD1(connection_borrow_message, DBusMessage*(DBusConnection *connection));

  MOCK_METHOD2(connection_return_message, void(DBusConnection *connection,
               DBusMessage *message));

  MOCK_METHOD1(connection_dispatch, DBusDispatchStatus(DBusConnection *connection));

  MOCK_METHOD4(connection_send_with_reply, dbus_bool_t(DBusConnection *connection,
                                                       DBusMessage *message,
                                                       DBusPendingCall **pending_return,
                                                       int timeout_milliseconds));

  MOCK_METHOD1(connection_flush, void(DBusConnection *connection));

  MOCK_METHOD2(connection_unregister_object_path, dbus_bool_t(DBusConnection *connection,
                                                              const char *path));

  MOCK_METHOD1(connection_unref, void(DBusConnection *connection));

  MOCK_METHOD1(error_init, void(DBusError *error));

  MOCK_METHOD1(error_free, void(DBusError *error));
};

}

}

}

#endif /* DBUS_H */
