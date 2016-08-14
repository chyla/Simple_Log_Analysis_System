/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <gtest/gtest.h>

#include <slas/dbus/detail/dbus_wrapper.h>
#include <slas/dbus/exception/dbus_exception.h>

#include "tests/mock/dbus/detail/dbus.h"

using namespace testing;
using namespace std;

#define EXAMPLE_CONNECTION_POINTER (reinterpret_cast<DBusConnection*>(0x01))
#define EXAMPLE_MESSAGE_POINTER (reinterpret_cast<DBusMessage*>(0x02))

class DBusWrapperTest : public ::testing::Test {
 public:

  void SetUp() {
    dbus = ::mock::dbus::detail::DBus::Create();
    dbus_wrapper = ::dbus::detail::DBusWrapper::Create(dbus);
    address = "example dbus address";
    bus_name = "example bus name";
    object_path = "example object path";
  }

  void TearDown() {
  }

  virtual ~DBusWrapperTest() {
  }

  ::mock::dbus::detail::DBusPtr dbus;
  ::dbus::detail::DBusWrapperPtr dbus_wrapper;
  std::string address, bus_name, object_path;
};

TEST_F(DBusWrapperTest, ConnectionOpenPrivate) {
  EXPECT_CALL(*dbus, error_init(NotNull()));
  EXPECT_CALL(*dbus, error_free(NotNull()));
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));

  dbus_wrapper->ConnectionOpenPrivate(address);
}

TEST_F(DBusWrapperTest, ConnectionOpenPrivate_WhenFail) {
  EXPECT_CALL(*dbus, error_init(NotNull()));
  EXPECT_CALL(*dbus, error_free(NotNull()));
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(nullptr));

  EXPECT_THROW(dbus_wrapper->ConnectionOpenPrivate(address), ::dbus::exception::DBusException);
}

TEST_F(DBusWrapperTest, BusRegister) {
  EXPECT_CALL(*dbus, error_init(NotNull())).Times(2);
  EXPECT_CALL(*dbus, error_free(NotNull())).Times(2);
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, bus_register(EXAMPLE_CONNECTION_POINTER, NotNull())).WillOnce(Return(true));

  dbus_wrapper->ConnectionOpenPrivate(address);
  dbus_wrapper->BusRegister();
}

TEST_F(DBusWrapperTest, BusRegister_WhenFail) {
  EXPECT_CALL(*dbus, error_init(NotNull())).Times(2);
  EXPECT_CALL(*dbus, error_free(NotNull())).Times(2);
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, bus_register(EXAMPLE_CONNECTION_POINTER, NotNull())).WillOnce(Return(false));

  dbus_wrapper->ConnectionOpenPrivate(address);
  EXPECT_THROW(dbus_wrapper->BusRegister(), ::dbus::exception::DBusException);
}

TEST_F(DBusWrapperTest, ConnectionClose) {
  EXPECT_CALL(*dbus, error_init(NotNull()));
  EXPECT_CALL(*dbus, error_free(NotNull()));
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, connection_close(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, connection_unref(EXAMPLE_CONNECTION_POINTER));

  dbus_wrapper->ConnectionOpenPrivate(address);
  dbus_wrapper->ConnectionClose();

  EXPECT_EQ(nullptr, dbus_wrapper->GetConnection());
}

TEST_F(DBusWrapperTest, BusRequestName) {
  EXPECT_CALL(*dbus, error_init(NotNull())).Times(2);
  EXPECT_CALL(*dbus, error_free(NotNull())).Times(2);
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, bus_request_name(EXAMPLE_CONNECTION_POINTER, StrEq(bus_name), 1, NotNull())).WillOnce(Return(0));

  dbus_wrapper->ConnectionOpenPrivate(address);
  dbus_wrapper->BusRequestName(bus_name, 1);
}

TEST_F(DBusWrapperTest, BusRequestName_WhenFail) {
  EXPECT_CALL(*dbus, error_init(NotNull())).Times(2);
  EXPECT_CALL(*dbus, error_free(NotNull())).Times(2);
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, bus_request_name(EXAMPLE_CONNECTION_POINTER, StrEq(bus_name), 1, NotNull())).WillOnce(Return(-1));

  dbus_wrapper->ConnectionOpenPrivate(address);
  EXPECT_THROW(dbus_wrapper->BusRequestName(bus_name, 1), ::dbus::exception::DBusException);
}

TEST_F(DBusWrapperTest, ConnectionRegisterObjectPath) {
  EXPECT_CALL(*dbus, error_init(NotNull())).Times(2);
  EXPECT_CALL(*dbus, error_free(NotNull())).Times(2);
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, connection_register_object_path(EXAMPLE_CONNECTION_POINTER, StrEq(object_path), nullptr, nullptr)).WillOnce(Return(true));

  dbus_wrapper->ConnectionOpenPrivate(address);
  dbus_wrapper->ConnectionRegisterObjectPath(object_path, nullptr, nullptr);
}

TEST_F(DBusWrapperTest, ConnectionRegisterObjectPath_WhenFail) {
  EXPECT_CALL(*dbus, error_init(NotNull())).Times(2);
  EXPECT_CALL(*dbus, error_free(NotNull())).Times(2);
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, connection_register_object_path(EXAMPLE_CONNECTION_POINTER, StrEq(object_path), nullptr, nullptr)).WillOnce(Return(false));

  dbus_wrapper->ConnectionOpenPrivate(address);
  EXPECT_THROW(dbus_wrapper->ConnectionRegisterObjectPath(object_path, nullptr, nullptr), ::dbus::exception::DBusException);
}

TEST_F(DBusWrapperTest, ConnectionReadWrite) {
  EXPECT_CALL(*dbus, error_init(NotNull())).Times(2);
  EXPECT_CALL(*dbus, error_free(NotNull())).Times(2);
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, connection_read_write(EXAMPLE_CONNECTION_POINTER, 3)).WillOnce(Return(true));

  dbus_wrapper->ConnectionOpenPrivate(address);
  dbus_wrapper->ConnectionReadWrite(3);
}

TEST_F(DBusWrapperTest, ConnectionReadWrite_WhenFail) {
  EXPECT_CALL(*dbus, error_init(NotNull())).Times(2);
  EXPECT_CALL(*dbus, error_free(NotNull())).Times(2);
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, connection_read_write(EXAMPLE_CONNECTION_POINTER, 2)).WillOnce(Return(false));

  dbus_wrapper->ConnectionOpenPrivate(address);
  EXPECT_THROW(dbus_wrapper->ConnectionReadWrite(2), ::dbus::exception::DBusException);
}

TEST_F(DBusWrapperTest, ConnectionBorrowMessage) {
  EXPECT_CALL(*dbus, error_init(NotNull()));
  EXPECT_CALL(*dbus, error_free(NotNull()));
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, connection_borrow_message(EXAMPLE_CONNECTION_POINTER)).WillOnce(Return(EXAMPLE_MESSAGE_POINTER));

  dbus_wrapper->ConnectionOpenPrivate(address);
  EXPECT_EQ(EXAMPLE_MESSAGE_POINTER, dbus_wrapper->ConnectionBorrowMessage());
}

TEST_F(DBusWrapperTest, ConnectionBorrowMessage_WhenFail) {
  EXPECT_CALL(*dbus, error_init(NotNull()));
  EXPECT_CALL(*dbus, error_free(NotNull()));
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, connection_borrow_message(EXAMPLE_CONNECTION_POINTER)).WillOnce(Return(nullptr));

  dbus_wrapper->ConnectionOpenPrivate(address);
  EXPECT_EQ(nullptr, dbus_wrapper->ConnectionBorrowMessage());
}

TEST_F(DBusWrapperTest, ConnectionSendWithReply) {
  DBusPendingCall *pending_call;

  EXPECT_CALL(*dbus, error_init(NotNull()));
  EXPECT_CALL(*dbus, error_free(NotNull()));
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, connection_send_with_reply(EXAMPLE_CONNECTION_POINTER, EXAMPLE_MESSAGE_POINTER, &pending_call, 4)).WillOnce(DoAll(SetArgPointee<2>(reinterpret_cast<DBusPendingCall*> (0x02)),
                                                                                                                                       Return(true)));

  dbus_wrapper->ConnectionOpenPrivate(address);
  dbus_wrapper->ConnectionSendWithReply(EXAMPLE_MESSAGE_POINTER, &pending_call, 4);
}

TEST_F(DBusWrapperTest, ConnectionSendWithReply_WhenFail) {
  DBusPendingCall *pending_call;

  EXPECT_CALL(*dbus, error_init(NotNull()));
  EXPECT_CALL(*dbus, error_free(NotNull()));
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, connection_send_with_reply(EXAMPLE_CONNECTION_POINTER, EXAMPLE_MESSAGE_POINTER, &pending_call, 4)).WillOnce(DoAll(SetArgPointee<2>(reinterpret_cast<DBusPendingCall*> (0x02)),
                                                                                                                                       Return(false)));

  dbus_wrapper->ConnectionOpenPrivate(address);
  EXPECT_THROW(dbus_wrapper->ConnectionSendWithReply(EXAMPLE_MESSAGE_POINTER, &pending_call, 4), ::dbus::exception::DBusException);
}

TEST_F(DBusWrapperTest, ConnectionSendWithReply_WhenReturnsNull) {
  DBusPendingCall *pending_call;

  EXPECT_CALL(*dbus, error_init(NotNull()));
  EXPECT_CALL(*dbus, error_free(NotNull()));
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, connection_send_with_reply(EXAMPLE_CONNECTION_POINTER, EXAMPLE_MESSAGE_POINTER, &pending_call, 4)).WillOnce(DoAll(SetArgPointee<2>(nullptr),
                                                                                                                                       Return(true)));

  dbus_wrapper->ConnectionOpenPrivate(address);
  EXPECT_THROW(dbus_wrapper->ConnectionSendWithReply(EXAMPLE_MESSAGE_POINTER, &pending_call, 4), ::dbus::exception::DBusException);
}

TEST_F(DBusWrapperTest, ConnectionUnregisterObjectPath) {
  EXPECT_CALL(*dbus, error_init(NotNull())).Times(2);
  EXPECT_CALL(*dbus, error_free(NotNull())).Times(2);
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, connection_unregister_object_path(EXAMPLE_CONNECTION_POINTER, StrEq(object_path))).WillOnce(Return(true));

  dbus_wrapper->ConnectionOpenPrivate(address);
  dbus_wrapper->ConnectionUnregisterObjectPath(object_path);
}

TEST_F(DBusWrapperTest, ConnectionUnregisterObjectPath_WhenFail) {
  EXPECT_CALL(*dbus, error_init(NotNull())).Times(2);
  EXPECT_CALL(*dbus, error_free(NotNull())).Times(2);
  EXPECT_CALL(*dbus, connection_open_private(StrEq(address), NotNull())).WillOnce(Return(EXAMPLE_CONNECTION_POINTER));
  EXPECT_CALL(*dbus, connection_unregister_object_path(EXAMPLE_CONNECTION_POINTER, StrEq(object_path))).WillOnce(Return(false));

  dbus_wrapper->ConnectionOpenPrivate(address);
  EXPECT_THROW(dbus_wrapper->ConnectionUnregisterObjectPath(object_path), ::dbus::exception::DBusException);
}
