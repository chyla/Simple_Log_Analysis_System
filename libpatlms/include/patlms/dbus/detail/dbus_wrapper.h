/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef DBUS_WRAPPER_H
#define DBUS_WRAPPER_H

#include <memory>

#include "dbus_wrapper_interface.h"
#include "dbus_interface.h"

namespace dbus
{

namespace detail
{

class DBusWrapper;
typedef std::shared_ptr<DBusWrapper> DBusWrapperPtr;

class DBusWrapper : public DBusWrapperInterface {
 public:
  virtual ~DBusWrapper() = default;

  static DBusWrapperPtr Create();
  static DBusWrapperPtr Create(DBusInterfacePtr dbus_interface);

  void ConnectionOpenPrivate(const std::string &address) override;

  void BusRegister() override;

  void ConnectionSetExitOnDisconnect(dbus_bool_t exit_on_disconnect) override;

  void ConnectionClose() override;

  int BusRequestName(const std::string &name,
                     unsigned int flags) override;

  void ConnectionRegisterObjectPath(const std::string &path,
                                    const DBusObjectPathVTable *vtable,
                                    void *user_data) override;

  void ConnectionReadWrite(int timeout_milliseconds) override;

  DBusMessage* ConnectionBorrowMessage() override;

  void ConnectionReturnMessage(DBusMessage *message) override;

  DBusDispatchStatus ConnectionDispatch() override;

  void ConnectionSendWithReply(DBusMessage *message,
                               DBusPendingCall **pending_return,
                               int timeout_milliseconds) override;

  void ConnectionFlush() override;

  void ConnectionUnregisterObjectPath(const std::string &path) override;

  DBusConnection* GetConnection() override;

 private:
  DBusWrapper(DBusInterfacePtr dbus_interface);

  void CheckForFalseThenThrowException(dbus_bool_t ret);
  void CheckForFalseThenThrowException(dbus_bool_t ret, const DBusError &error);
  std::string GetErrorMessage(const DBusError &error);

  DBusInterfacePtr dbus_interface_;
  DBusConnection *connection_;
  DBusError error_;
};

}

}

#endif /* DBUS_WRAPPER_H */
