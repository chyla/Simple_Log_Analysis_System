/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include <memory>
#include <string>
#include <dbus/dbus.h>

namespace dbus
{

namespace detail
{

class DBusWrapperInterface {
 public:
  virtual ~DBusWrapperInterface() = default;

  virtual void ConnectionOpenPrivate(const std::string &address) = 0;

  virtual void BusRegister() = 0;

  virtual void ConnectionSetExitOnDisconnect(dbus_bool_t exit_on_disconnect) = 0;

  virtual void ConnectionClose() = 0;

  virtual int BusRequestName(const std::string &name,
                             unsigned int flags) = 0;

  virtual void ConnectionRegisterObjectPath(const std::string &path,
                                            const DBusObjectPathVTable *vtable,
                                            void *user_data) = 0;

  virtual void ConnectionReadWrite(int timeout_milliseconds) = 0;

  virtual DBusMessage* ConnectionBorrowMessage() = 0;

  virtual void ConnectionReturnMessage(DBusMessage *message) = 0;

  virtual DBusDispatchStatus ConnectionDispatch() = 0;

  virtual void ConnectionSendWithReply(DBusMessage *message,
                                       DBusPendingCall **pending_return,
                                       int timeout_milliseconds) = 0;

  virtual void ConnectionFlush() = 0;

  virtual void ConnectionUnregisterObjectPath(const std::string &path) = 0;

  virtual DBusConnection* GetConnection() = 0;
};

typedef std::shared_ptr<DBusWrapperInterface> DBusWrapperInterfacePtr;

}

}
