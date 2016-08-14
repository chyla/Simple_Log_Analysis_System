/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <boost/log/trivial.hpp>

#include <slas/dbus/detail/dbus.h>
#include <slas/dbus/detail/dbus_wrapper.h>
#include <slas/dbus/detail/dbus_error_guard.h>
#include <slas/dbus/exception/dbus_exception.h>

namespace dbus
{

namespace detail
{

DBusWrapperPtr DBusWrapper::Create() {
  auto ptr = DBus::Create();

  return DBusWrapper::Create(ptr);
}

DBusWrapperPtr DBusWrapper::Create(DBusInterfacePtr dbus_interface) {
  return DBusWrapperPtr(new DBusWrapper(dbus_interface));
}

void DBusWrapper::ConnectionOpenPrivate(const std::string &address) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBusWrapper::ConnectionOpenPrivate: Function call";

  DBusErrorGuard error_guard(dbus_interface_, &error_);

  connection_ = dbus_interface_->connection_open_private(address.c_str(), &error_);
  if (connection_ == nullptr) {
    BOOST_LOG_TRIVIAL(error) << "dbus::detail::DBusWrapper::ConnectionOpenPrivate: Failed to connect to the bus: " << GetErrorMessage(error_);
    throw ::dbus::exception::DBusException();
  }
}

void DBusWrapper::BusRegister() {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBusWrapper::BusRegister: Function call";

  DBusErrorGuard error_guard(dbus_interface_, &error_);

  auto ret = dbus_interface_->bus_register(connection_, &error_);
  CheckForFalseThenThrowException(ret, error_);
}

void DBusWrapper::ConnectionSetExitOnDisconnect(dbus_bool_t exit_on_disconnect) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBusWrapper::BusRegister: Function call (exit_on_disconnect=" << exit_on_disconnect;

  dbus_interface_->connection_set_exit_on_disconnect(connection_, exit_on_disconnect);
}

void DBusWrapper::ConnectionClose() {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBusWrapper::ConnectionClose: Function call";

  dbus_interface_->connection_close(connection_);
  dbus_interface_->connection_unref(connection_);

  connection_ = nullptr;
}

int DBusWrapper::BusRequestName(const std::string &name,
                                unsigned int flags) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBusWrapper::BusRequestName: Function call";

  DBusErrorGuard error_guard(dbus_interface_, &error_);

  auto ret = dbus_interface_->bus_request_name(connection_, name.c_str(), flags, &error_);
  if (ret < 0)
    CheckForFalseThenThrowException(false, error_);

  return ret;
}

void DBusWrapper::ConnectionRegisterObjectPath(const std::string &path,
                                               const DBusObjectPathVTable *vtable,
                                               void *user_data) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBusWrapper::ConnectionRegisterObjectPath: Function call";

  DBusErrorGuard error_guard(dbus_interface_, &error_);

  auto ret = dbus_interface_->connection_register_object_path(connection_, path.c_str(), vtable, user_data);
  CheckForFalseThenThrowException(ret, error_);
}

void DBusWrapper::ConnectionReadWrite(int timeout_milliseconds) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBusWrapper::ConnectionReadWrite: Function call";

  DBusErrorGuard error_guard(dbus_interface_, &error_);

  auto ret = dbus_interface_->connection_read_write(connection_, timeout_milliseconds);
  CheckForFalseThenThrowException(ret, error_);
}

DBusMessage* DBusWrapper::ConnectionBorrowMessage() {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBusWrapper::ConnectionBorrowMessage: Function call";

  auto msg = dbus_interface_->connection_borrow_message(connection_);
  if (msg == nullptr)
    BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBusWrapper::ConnectionBorrowMessage: Message pointer is null";

  return msg;
}

void DBusWrapper::ConnectionReturnMessage(DBusMessage *message) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBusWrapper::ConnectionReturnMessage: Function call";

  dbus_interface_->connection_return_message(connection_, message);
}

DBusDispatchStatus DBusWrapper::ConnectionDispatch() {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBusWrapper::ConnectionDispatch: Function call";

  return dbus_interface_->connection_dispatch(connection_);
}

void DBusWrapper::ConnectionSendWithReply(DBusMessage *message,
                                          DBusPendingCall **pending_return,
                                          int timeout_milliseconds) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBusWrapper::ConnectionSendWithReply: Function call";

  auto ret = dbus_interface_->connection_send_with_reply(connection_, message, pending_return, timeout_milliseconds);
  CheckForFalseThenThrowException(ret);

  if (*pending_return == nullptr) {
    BOOST_LOG_TRIVIAL(error) << "dbus::detail::DBusWrapper::ConnectionSendWithReply: pending_return is null";
    throw ::dbus::exception::DBusException();
  }
}

void DBusWrapper::ConnectionFlush() {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBusWrapper::ConnectionFlush: Function call";

  dbus_interface_->connection_flush(connection_);
}

void DBusWrapper::ConnectionUnregisterObjectPath(const std::string &path) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::detail::DBusWrapper::ConnectionUnregisterObjectPath: Function call";

  DBusErrorGuard error_guard(dbus_interface_, &error_);

  auto ret = dbus_interface_->connection_unregister_object_path(connection_, path.c_str());
  CheckForFalseThenThrowException(ret, error_);
}

DBusConnection* DBusWrapper::GetConnection() {
  return connection_;
}

DBusWrapper::DBusWrapper(DBusInterfacePtr dbus_interface) :
dbus_interface_(dbus_interface),
connection_(nullptr) {
}

void DBusWrapper::CheckForFalseThenThrowException(dbus_bool_t ret) {
  if (ret == false) {
    BOOST_LOG_TRIVIAL(error) << "dbus::detail::DBusWrapper::CheckForFalseThenThrowException: DBus error";
    throw ::dbus::exception::DBusException();
  }
}

void DBusWrapper::CheckForFalseThenThrowException(dbus_bool_t ret, const DBusError &error) {
  if (ret == false) {
    BOOST_LOG_TRIVIAL(error) << "dbus::detail::DBusWrapper::CheckForFalseThenThrowException: DBus error: " << GetErrorMessage(error_);
    throw ::dbus::exception::DBusException();
  }
}

std::string DBusWrapper::GetErrorMessage(const DBusError &error) {
  return error.message != nullptr ? error.message : "";
}

}

}
