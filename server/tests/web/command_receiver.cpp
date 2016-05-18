#include <gmock/gmock.h>
#include <memory>
#include <thread>
#include <unistd.h>

#include "tests/mock/libpatlms/network/network.h"
#include "tests/mock/web/command_executor.h"

#include "src/web/command_receiver.h"
#include "src/web/exception/detail/connection_is_already_open_exception.h"
#include "src/web/exception/detail/port_is_closed_exception.h"

using namespace testing;
using namespace std;
using namespace network;
using namespace web;

class CommandReceiverTest : public ::testing::Test {
 public:

  void SetUp() {
    network_mock = make_shared<mock::libpatlms::network::Network>();
    command_executor_mock = make_shared<mock::web::CommandExecutor>();
    command_receiver = CommandReceiver::Create(command_executor_mock, network_mock);
  }

  void TearDown() {
  }

  virtual ~CommandReceiverTest() {
  }

  shared_ptr<mock::libpatlms::network::Network> network_mock;
  shared_ptr<mock::web::CommandExecutor> command_executor_mock;
  CommandReceiverPtr command_receiver;

  constexpr static auto ADDRESS = "127.0.0.1";
  constexpr static auto PORT = 8088;
  constexpr static auto SOCKET = 34;
  constexpr static auto CLIENT_SOCKET = 13;
  constexpr static auto TIMEOUT = 3;
};

TEST_F(CommandReceiverTest, IsPortOpen_WhenConstructed) {
  EXPECT_FALSE(command_receiver->IsPortOpen());
}

TEST_F(CommandReceiverTest, OpenPort) {
  EXPECT_CALL(*network_mock, OpenIpv4Socket(StrEq(ADDRESS), PORT)).WillOnce(Return(SOCKET));

  command_receiver->OpenPort(ADDRESS, PORT);

  EXPECT_TRUE(command_receiver->IsPortOpen());
}

TEST_F(CommandReceiverTest, OpenPort_WhenCalledSecondTime) {
  EXPECT_CALL(*network_mock, OpenIpv4Socket(StrEq(ADDRESS), PORT)).WillOnce(Return(SOCKET));

  command_receiver->OpenPort(ADDRESS, PORT);

  EXPECT_THROW(command_receiver->OpenPort(ADDRESS, PORT), exception::detail::ConnectionIsAlreadyOpenException);
  EXPECT_TRUE(command_receiver->IsPortOpen());
}

TEST_F(CommandReceiverTest, ClosePort) {
  EXPECT_CALL(*network_mock, OpenIpv4Socket(StrEq(ADDRESS), PORT)).WillOnce(Return(SOCKET));
  EXPECT_CALL(*network_mock, Close(SOCKET));

  command_receiver->OpenPort(ADDRESS, PORT);
  command_receiver->ClosePort();
  
  EXPECT_FALSE(command_receiver->IsPortOpen());  
}

TEST_F(CommandReceiverTest, ClosePort_whenPortIsClosed) {
  EXPECT_FALSE(command_receiver->IsPortOpen());  

  EXPECT_THROW(command_receiver->ClosePort(), exception::detail::PortIsClosedException);
  
  EXPECT_FALSE(command_receiver->IsPortOpen());  
}

TEST_F(CommandReceiverTest, IsListen_WhenConstructed) {
  EXPECT_FALSE(command_receiver->IsListen());
}

TEST_F(CommandReceiverTest, StartListen) {
  network::ConnectionData new_client;
  new_client.socket = CLIENT_SOCKET;
  string example_command_text = "example command";
  string example_command_result = "example command result";

  EXPECT_CALL(*network_mock, OpenIpv4Socket(StrEq(ADDRESS), PORT)).WillOnce(Return(SOCKET));
  EXPECT_CALL(*network_mock, Accept(SOCKET)).WillRepeatedly(Return(new_client));
  EXPECT_CALL(*network_mock, WaitForData(SOCKET, TIMEOUT)).WillRepeatedly(Return(network::WaitStatus::NEW_DATA));
  EXPECT_CALL(*network_mock, WaitForData(CLIENT_SOCKET, TIMEOUT)).WillRepeatedly(Return(network::WaitStatus::NEW_DATA));
  EXPECT_CALL(*network_mock, ReceiveText(CLIENT_SOCKET)).WillRepeatedly(Return(example_command_text));
  EXPECT_CALL(*network_mock, SendText(CLIENT_SOCKET, example_command_result)).WillRepeatedly(Return());
  EXPECT_CALL(*network_mock, Close(CLIENT_SOCKET)).WillRepeatedly(Return());

  EXPECT_CALL(*command_executor_mock, Execute(example_command_text)).WillRepeatedly(Return(example_command_result));

  command_receiver->OpenPort(ADDRESS, PORT);

  thread listen_thread([this]() {
    command_receiver->StartListen();
  });

  usleep(300);

  command_receiver->StopListen();

  listen_thread.join();
}

TEST_F(CommandReceiverTest, StartListen_WhenPortIsClosed) {
  EXPECT_FALSE(command_receiver->IsPortOpen());

  EXPECT_THROW(command_receiver->StartListen(), exception::detail::PortIsClosedException);
}
