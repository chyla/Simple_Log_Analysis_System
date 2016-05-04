#include <memory>
#include <gmock/gmock.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "src/bash/bash_log_receiver.h"
#include "src/bash/exception/detail/cant_open_socket_exception.h"
#include "src/bash/exception/detail/loop_poll_exception.h"
#include "src/bash/exception/detail/loop_recv_exception.h"
#include "src/bash/exception/detail/loop_accept_exception.h"
#include "src/bash/exception/detail/loop_getsockopt_exception.h"
#include "src/bash/exception/detail/loop_time_exception.h"
#include "src/bash/exception/detail/loop_gmtime_exception.h"
#include "src/bash/exception/detail/loop_gethostname_exception.h"
#include "src/bash/exception/detail/socket_is_not_open_exception.h"

#include "tests/mock/bash/detail/system.h"
#include "tests/mock/dbus/bus.h"
#include "tests/mock/dbus/dbus_thread.h"

using namespace std;
using namespace bash;
using namespace testing;

constexpr int SOCKET_FD = 3;
constexpr int TIMEOUT = 5 * 1000;

#define EXAMPLE_PTR_VALUE (reinterpret_cast<struct tm*>(0x000001))

class BashLogReceiverTest : public ::testing::Test {
 public:

  void SetUp() {
    bus = make_shared<mock::dbus::Bus>();
    dbus_thread = make_shared<mock::dbus::DBusThread>();
    system = make_shared<mock::bash::System>();
  }

  void TearDown() {
  }

  virtual ~BashLogReceiverTest() {
  }

  shared_ptr<dbus::detail::BusInterface> bus;
  shared_ptr<mock::dbus::DBusThread> dbus_thread;
  shared_ptr<mock::bash::System> system;

  const char *socket_path = "/tmp/bash-mod.sock";
};

TEST_F(BashLogReceiverTest, LoopNotRunningWhenObjectIsCreated) {
  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);

  EXPECT_FALSE(receiver->IsRunning());
}

TEST_F(BashLogReceiverTest, OpenSocket) {
  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);
  EXPECT_CALL(*system, Unlink(StrEq(socket_path))).WillOnce(Return(0));
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(SOCKET_FD));
  EXPECT_CALL(*system, Bind(SOCKET_FD, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(SOCKET_FD, 20)).WillOnce(Return(0));
  EXPECT_CALL(*system, Chmod(StrEq(socket_path), 0622)).WillOnce(Return(0));

  receiver->OpenSocket(socket_path);
}

TEST_F(BashLogReceiverTest, OpenSocketWhenUnlinkFailed) {
  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);
  EXPECT_CALL(*system, Unlink(StrEq(socket_path))).WillOnce(Return(-1));
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(SOCKET_FD));
  EXPECT_CALL(*system, Bind(SOCKET_FD, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(SOCKET_FD, 20)).WillOnce(Return(0));
  EXPECT_CALL(*system, Chmod(StrEq(socket_path), 0622)).WillOnce(Return(0));

  receiver->OpenSocket(socket_path);
}

TEST_F(BashLogReceiverTest, OpenSocketWhenSocketFailed) {
  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);
  EXPECT_CALL(*system, Unlink(StrEq(socket_path))).WillOnce(Return(0));
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(-1));

  EXPECT_THROW(receiver->OpenSocket(socket_path), exception::detail::CantOpenSocketException);
}

TEST_F(BashLogReceiverTest, OpenSocketWhenBindFailed) {
  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);
  EXPECT_CALL(*system, Unlink(StrEq(socket_path))).WillOnce(Return(0));
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(SOCKET_FD));
  EXPECT_CALL(*system, Bind(SOCKET_FD, _, _)).WillOnce(Return(-1));

  EXPECT_THROW(receiver->OpenSocket(socket_path), exception::detail::CantOpenSocketException);
}

TEST_F(BashLogReceiverTest, OpenSocketWhenListenFailed) {
  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);
  EXPECT_CALL(*system, Unlink(StrEq(socket_path))).WillOnce(Return(0));
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(SOCKET_FD));
  EXPECT_CALL(*system, Bind(SOCKET_FD, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(SOCKET_FD, 20)).WillOnce(Return(-1));

  EXPECT_THROW(receiver->OpenSocket(socket_path), exception::detail::CantOpenSocketException);
}

TEST_F(BashLogReceiverTest, OpenSocketWhenChmodFailed) {
  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);
  EXPECT_CALL(*system, Unlink(StrEq(socket_path))).WillOnce(Return(0));
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(SOCKET_FD));
  EXPECT_CALL(*system, Bind(SOCKET_FD, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(SOCKET_FD, 20)).WillOnce(Return(0));
  EXPECT_CALL(*system, Chmod(StrEq(socket_path), 0622)).WillOnce(Return(-1));

  EXPECT_THROW(receiver->OpenSocket(socket_path), exception::detail::CantOpenSocketException);
}

TEST_F(BashLogReceiverTest, StartLoopWithoutOpenSocket) {
  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);
  ASSERT_FALSE(receiver->IsRunning());

  EXPECT_THROW(receiver->StartLoop(), exception::detail::SocketIsNotOpenException);
  EXPECT_FALSE(receiver->IsRunning());
}

TEST_F(BashLogReceiverTest, StartLoopWhenPollFailed) {
  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);
  EXPECT_CALL(*system, Unlink(StrEq(socket_path))).WillOnce(Return(0));
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(SOCKET_FD));
  EXPECT_CALL(*system, Bind(SOCKET_FD, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(SOCKET_FD, 20)).WillOnce(Return(0));
  EXPECT_CALL(*system, Chmod(StrEq(socket_path), 0622)).WillOnce(Return(0));
  EXPECT_CALL(*system, Poll(NotNull(), Eq(1), TIMEOUT)).WillOnce(Return(-1));

  receiver->OpenSocket(socket_path);

  EXPECT_THROW(receiver->StartLoop(), exception::detail::LoopPollException);
}

TEST_F(BashLogReceiverTest, StartLoopWhenAcceptFailed) {
  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);
  EXPECT_CALL(*system, Unlink(StrEq(socket_path))).WillOnce(Return(0));
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(SOCKET_FD));
  EXPECT_CALL(*system, Bind(SOCKET_FD, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(SOCKET_FD, 20)).WillOnce(Return(0));
  EXPECT_CALL(*system, Chmod(StrEq(socket_path), 0622)).WillOnce(Return(0));
  EXPECT_CALL(*system, Poll(NotNull(), Eq(1), TIMEOUT)).WillOnce(Return(1));
  EXPECT_CALL(*system, Accept(SOCKET_FD, _, _)).WillOnce(Return(-1));

  receiver->OpenSocket(socket_path);

  EXPECT_THROW(receiver->StartLoop(), exception::detail::LoopAcceptException);
}

TEST_F(BashLogReceiverTest, StartLoopWhenGetsockoptFailed) {
  constexpr int new_fd = 95;

  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);
  EXPECT_CALL(*system, Unlink(StrEq(socket_path))).WillOnce(Return(0));
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(SOCKET_FD));
  EXPECT_CALL(*system, Bind(SOCKET_FD, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(SOCKET_FD, 20)).WillOnce(Return(0));
  EXPECT_CALL(*system, Chmod(StrEq(socket_path), 0622)).WillOnce(Return(0));
  EXPECT_CALL(*system, Poll(NotNull(), Eq(1), TIMEOUT)).WillOnce(Return(1));
  EXPECT_CALL(*system, Accept(SOCKET_FD, _, _)).WillOnce(Return(new_fd));
  EXPECT_CALL(*system, Getsockopt(new_fd, SOL_SOCKET, SO_PEERCRED, _, _)).WillOnce(Return(-1));

  receiver->OpenSocket(socket_path);

  EXPECT_THROW(receiver->StartLoop(), exception::detail::LoopGetsockoptException);
}

TEST_F(BashLogReceiverTest, StartLoopWhenRecvFailed) {
  constexpr int new_fd = 95;

  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);
  EXPECT_CALL(*system, Unlink(StrEq(socket_path))).WillOnce(Return(0));
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(SOCKET_FD));
  EXPECT_CALL(*system, Bind(SOCKET_FD, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(SOCKET_FD, 20)).WillOnce(Return(0));
  EXPECT_CALL(*system, Chmod(StrEq(socket_path), 0622)).WillOnce(Return(0));
  EXPECT_CALL(*system, Poll(NotNull(), Eq(1), TIMEOUT)).WillOnce(Return(1));
  EXPECT_CALL(*system, Accept(SOCKET_FD, _, _)).WillOnce(Return(new_fd));
  EXPECT_CALL(*system, Getsockopt(new_fd, SOL_SOCKET, SO_PEERCRED, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Recv(new_fd, _, _, 0)).WillOnce(Return(-1));

  receiver->OpenSocket(socket_path);

  EXPECT_THROW(receiver->StartLoop(), exception::detail::LoopRecvException);
}

TEST_F(BashLogReceiverTest, StartLoopWhenTimeFailed) {
  constexpr int new_fd = 95;

  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);
  EXPECT_CALL(*system, Unlink(StrEq(socket_path))).WillOnce(Return(0));
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(SOCKET_FD));
  EXPECT_CALL(*system, Bind(SOCKET_FD, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(SOCKET_FD, 20)).WillOnce(Return(0));
  EXPECT_CALL(*system, Chmod(StrEq(socket_path), 0622)).WillOnce(Return(0));
  EXPECT_CALL(*system, Poll(NotNull(), Eq(1), TIMEOUT)).WillOnce(Return(1));
  EXPECT_CALL(*system, Accept(SOCKET_FD, _, _)).WillOnce(Return(new_fd));
  EXPECT_CALL(*system, Getsockopt(new_fd, SOL_SOCKET, SO_PEERCRED, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Recv(new_fd, _, _, 0)).WillOnce(Return(1));
  EXPECT_CALL(*system, Time(nullptr)).WillOnce(Return((time_t) (-1)));

  receiver->OpenSocket(socket_path);

  EXPECT_THROW(receiver->StartLoop(), exception::detail::LoopTimeException);
}

TEST_F(BashLogReceiverTest, StartLoopWhenGMTimeFailed) {
  constexpr int new_fd = 95;

  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);
  EXPECT_CALL(*system, Unlink(StrEq(socket_path))).WillOnce(Return(0));
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(SOCKET_FD));
  EXPECT_CALL(*system, Bind(SOCKET_FD, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(SOCKET_FD, 20)).WillOnce(Return(0));
  EXPECT_CALL(*system, Chmod(StrEq(socket_path), 0622)).WillOnce(Return(0));
  EXPECT_CALL(*system, Poll(NotNull(), Eq(1), TIMEOUT)).WillOnce(Return(1));
  EXPECT_CALL(*system, Accept(SOCKET_FD, _, _)).WillOnce(Return(new_fd));
  EXPECT_CALL(*system, Getsockopt(new_fd, SOL_SOCKET, SO_PEERCRED, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Recv(new_fd, _, _, 0)).WillOnce(Return(1));
  EXPECT_CALL(*system, Time(nullptr)).WillOnce(Return(1));
  EXPECT_CALL(*system, GMTime(_)).WillOnce(Return(nullptr));

  receiver->OpenSocket(socket_path);

  EXPECT_THROW(receiver->StartLoop(), exception::detail::LoopGMTimeException);
}

TEST_F(BashLogReceiverTest, StartLoopWhenGethostnameFailed) {
  constexpr int new_fd = 95;

  shared_ptr<BashLogReceiver> receiver = BashLogReceiver::Create(bus, dbus_thread, system);
  EXPECT_CALL(*system, Unlink(StrEq(socket_path))).WillOnce(Return(0));
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(SOCKET_FD));
  EXPECT_CALL(*system, Bind(SOCKET_FD, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(SOCKET_FD, 20)).WillOnce(Return(0));
  EXPECT_CALL(*system, Chmod(StrEq(socket_path), 0622)).WillOnce(Return(0));
  EXPECT_CALL(*system, Poll(NotNull(), Eq(1), TIMEOUT)).WillOnce(Return(1));
  EXPECT_CALL(*system, Accept(SOCKET_FD, _, _)).WillOnce(Return(new_fd));
  EXPECT_CALL(*system, Getsockopt(new_fd, SOL_SOCKET, SO_PEERCRED, _, _)).WillOnce(Return(0));
  EXPECT_CALL(*system, Recv(new_fd, _, _, 0)).WillOnce(Return(1));
  EXPECT_CALL(*system, Time(nullptr)).WillOnce(Return(1));
  EXPECT_CALL(*system, GMTime(_)).WillOnce(Return(EXAMPLE_PTR_VALUE));
  EXPECT_CALL(*system, Gethostname(_, _)).WillOnce(Return(-1));

  receiver->OpenSocket(socket_path);

  EXPECT_THROW(receiver->StartLoop(), exception::detail::LoopGethostnameException);
}
