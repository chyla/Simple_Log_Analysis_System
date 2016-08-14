#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <poll.h>

#include <slas/network/network.h>
#include <slas/network/wait_status.h>
#include <slas/network/detail/system_interface.h>
#include <slas/network/exception/detail/cant_open_socket_exception.h>
#include <slas/network/exception/detail/bad_address_exception.h>
#include <slas/network/exception/detail/close_exception.h>
#include <slas/network/exception/detail//poll_exception.h>
#include <slas/network/exception/detail/accept_exception.h>
#include <slas/network/exception/detail/timeout_exception.h>
#include <slas/network/exception/detail/message_too_long_exception.h>
#include <slas/network/exception/detail/connect_exception.h>

#include "tests/mock/network/detail/system.h"

using namespace testing;
using namespace network;
using namespace std;

class NetworkTest : public ::testing::Test {
 public:

  void SetUp() {
    system = std::make_shared<mock::network::detail::System>();
  }

  void TearDown() {
  }

  virtual ~NetworkTest() {
  }

  std::shared_ptr<mock::network::detail::System> system;
};

TEST_F(NetworkTest, SocketDefault) {
  EXPECT_CALL(*system, Socket(PF_INET, SOCK_STREAM, 0)).WillOnce(Return(13));
  NetworkPtr network = Network::Create(system);

  EXPECT_EQ(13, network->Socket());
}

TEST_F(NetworkTest, SocketUnix) {
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(13));
  NetworkPtr network = Network::Create(system);

  EXPECT_EQ(13, network->Socket(PF_UNIX));
}

TEST_F(NetworkTest, SocketDefaultWhenSocketFail) {
  EXPECT_CALL(*system, Socket(PF_INET, SOCK_STREAM, 0)).WillOnce(Return(-1));
  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->Socket(), exception::detail::CantOpenSocketException);
}

TEST_F(NetworkTest, CreateUnixSocket) {
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(13));
  EXPECT_CALL(*system, Bind(_, _, sizeof (struct sockaddr_un))).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(_, 20)).WillOnce(Return(0));
  NetworkPtr network = Network::Create(system);

  EXPECT_EQ(13, network->OpenUnixSocket("/var/run/test_unix.socket"));
}

TEST_F(NetworkTest, CreateUnixSocketWhenSocketFailed) {
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(-1));
  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->OpenUnixSocket("/var/run/test_unix.socket"), exception::detail::CantOpenSocketException);
}

TEST_F(NetworkTest, CreateUnixSocketWhenBindFailed) {
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(13));
  EXPECT_CALL(*system, Bind(_, _, sizeof (struct sockaddr_un))).WillOnce(Return(-1));
  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->OpenUnixSocket("/var/run/test_unix.socket"), exception::detail::CantOpenSocketException);
}

TEST_F(NetworkTest, CreateUnixSocketWhenListenFailed) {
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(13));
  EXPECT_CALL(*system, Bind(_, _, sizeof (struct sockaddr_un))).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(_, 20)).WillOnce(Return(-1));
  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->OpenUnixSocket("/var/run/test_unix.socket"), exception::detail::CantOpenSocketException);
}

TEST_F(NetworkTest, CreateUnixSocketWithMaxPathLength) {
  EXPECT_CALL(*system, Socket(PF_UNIX, SOCK_STREAM, 0)).WillOnce(Return(13));
  EXPECT_CALL(*system, Bind(_, _, sizeof (struct sockaddr_un))).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(_, 20)).WillOnce(Return(0));

  string path = "";
  for (int i = 0; i < 90; i++)
    path += "a";

  ASSERT_EQ(90, path.length());

  NetworkPtr network = Network::Create(system);

  EXPECT_EQ(13, network->OpenUnixSocket(path));
}

TEST_F(NetworkTest, CreateUnixSocketWhenPathIsTooLong) {
  string path = "";
  for (int i = 0; i < 91; i++)
    path += "a";

  ASSERT_EQ(91, path.length());

  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->OpenUnixSocket(path.c_str()), exception::detail::BadAddressException);
}

TEST_F(NetworkTest, OpenIpv4SocketW) {
  EXPECT_CALL(*system, Socket(PF_INET, SOCK_STREAM, 0)).WillOnce(Return(13));
  EXPECT_CALL(*system, Bind(_, _, sizeof (struct sockaddr_in))).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(_, 20)).WillOnce(Return(0));
  NetworkPtr network = Network::Create(system);

  EXPECT_EQ(13, network->OpenIpv4Socket("127.0.0.1", 90));
}

TEST_F(NetworkTest, OpenIpv4SocketWhenAddressContainsWrongChars) {
  NetworkPtr network = Network::Create(system);
  EXPECT_THROW(network->OpenIpv4Socket("127,1.0.1", 90), exception::detail::BadAddressException);
}

TEST_F(NetworkTest, OpenIpv4SocketWhenSocketFailed) {
  EXPECT_CALL(*system, Socket(PF_INET, SOCK_STREAM, 0)).WillOnce(Return(-1));
  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->OpenIpv4Socket("127.1.0.1", 90), exception::detail::CantOpenSocketException);
}

TEST_F(NetworkTest, OpenIpv4SocketWhenBindFailed) {
  EXPECT_CALL(*system, Socket(PF_INET, SOCK_STREAM, 0)).WillOnce(Return(13));
  EXPECT_CALL(*system, Bind(_, _, sizeof (struct sockaddr_in))).WillOnce(Return(-1));
  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->OpenIpv4Socket("127.1.0.1", 90), exception::detail::CantOpenSocketException);
}

TEST_F(NetworkTest, OpenIpv4SocketWhenListenFailed) {
  EXPECT_CALL(*system, Socket(PF_INET, SOCK_STREAM, 0)).WillOnce(Return(13));
  EXPECT_CALL(*system, Bind(_, _, sizeof (struct sockaddr_in))).WillOnce(Return(0));
  EXPECT_CALL(*system, Listen(_, 20)).WillOnce(Return(-1));
  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->OpenIpv4Socket("127.1.0.1", 90), exception::detail::CantOpenSocketException);
}

TEST_F(NetworkTest, ConnectUnix) {
  EXPECT_CALL(*system, Connect(13, _, _)).WillOnce(Return(0));
  NetworkPtr network = Network::Create(system);

  network->ConnectUnix(13, "/socket");
}

TEST_F(NetworkTest, ConnectUnixWhenConnectFail) {
  EXPECT_CALL(*system, Connect(13, _, sizeof (struct sockaddr_un))).WillOnce(Return(-1));
  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->ConnectUnix(13, "/socket"), exception::detail::ConnectException);
}

TEST_F(NetworkTest, Close) {
  EXPECT_CALL(*system, Close(13)).WillOnce(Return(0));
  NetworkPtr network = Network::Create(system);

  network->Close(13);
}

TEST_F(NetworkTest, CloseWhenCloseFailed) {
  EXPECT_CALL(*system, Close(13)).WillOnce(Return(-1));
  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->Close(13), exception::detail::CloseException);
}

TEST_F(NetworkTest, WaitForDataTimeout) {
  EXPECT_CALL(*system, Poll(_, 1, 2000)).WillOnce(Return(0));
  NetworkPtr network = Network::Create(system);

  EXPECT_EQ(WaitStatus::TIMEOUT, network->WaitForData(0, 2));
}

TEST_F(NetworkTest, WaitForDataNewData) {
  EXPECT_CALL(*system, Poll(_, 1, 2000)).WillOnce(Return(1));
  NetworkPtr network = Network::Create(system);

  EXPECT_EQ(WaitStatus::NEW_DATA, network->WaitForData(0, 2));
}

TEST_F(NetworkTest, WaitForDataOnPullError) {
  EXPECT_CALL(*system, Poll(_, 1, 2000)).WillOnce(Return(-1));
  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->WaitForData(0, 2), exception::detail::PollException);
}

TEST_F(NetworkTest, Accept) {
  EXPECT_CALL(*system, Accept(13, _, _)).WillOnce(Return(14));
  NetworkPtr network = Network::Create(system);

  ConnectionData data = network->Accept(13);

  EXPECT_EQ(14, data.socket);
}

TEST_F(NetworkTest, AcceptWhenAcceptFail) {
  EXPECT_CALL(*system, Accept(13, _, _)).WillOnce(Return(-1));
  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->Accept(13), exception::detail::AcceptException);
}

TEST_F(NetworkTest, RecvMessageOnePart) {
  EXPECT_CALL(*system, Recv(13, _, 1, 0)).WillOnce(Invoke([](int, void *buffer, size_t, int) {
    char *buf = (char*) buffer;
    buf[0] = 5;
    return 1;
  }));
  EXPECT_CALL(*system, Recv(13, _, 5, 0)).WillOnce(Invoke([](int, void *buffer, size_t, int) {
    char *buf = (char*) buffer;
    strncpy(buf, "test", 5);
    return 5;
  }));
  EXPECT_CALL(*system, Poll(_, 1, detail::TimeoutSeconds * 1000))
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  NetworkPtr network = Network::Create(system);

  NetworkMessage nm = network->RecvMessage(13);
  ASSERT_EQ(5, nm.size());
  EXPECT_STREQ(nm.data(), "test");
}

TEST_F(NetworkTest, RecvMessageTwoParts) {
  EXPECT_CALL(*system, Recv(13, _, 1, 0))
      .WillOnce(Invoke([](int, void *buffer, size_t, int) {
        char *buf = (char*) buffer;
        buf[0] = 5;
        return 1;
      }));
  EXPECT_CALL(*system, Recv(13, _, 5, 0))
      .WillOnce(Invoke([](int, void *buffer, size_t, int) {
        char *buf = (char*) buffer;
        strncpy(buf, "te", 2);
        return 2;
      }));
  EXPECT_CALL(*system, Recv(13, _, 3, 0))
      .WillOnce(Invoke([](int, void *buffer, size_t, int) {
        char *buf = (char*) buffer;
        strncpy(buf, "st", 3);
        return 3;
      }));
  EXPECT_CALL(*system, Poll(_, 1, detail::TimeoutSeconds * 1000))
      .WillOnce(Return(1))
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  NetworkPtr network = Network::Create(system);

  NetworkMessage nm = network->RecvMessage(13);
  ASSERT_EQ(5, nm.size());
  EXPECT_STREQ(nm.data(), "test");
}

TEST_F(NetworkTest, RecvMessageThreeParts) {
  EXPECT_CALL(*system, Recv(13, _, 1, 0))
      .WillOnce(Invoke([](int, void *buffer, size_t, int) {
        char *buf = (char*) buffer;
        buf[0] = 8;
        return 1;
      }));
  EXPECT_CALL(*system, Recv(13, _, 8, 0))
      .WillOnce(Invoke([](int, void *buffer, size_t, int) {
        char *buf = (char*) buffer;
        strncpy(buf, "te", 2);
        return 2;
      }));
  EXPECT_CALL(*system, Recv(13, _, 6, 0))
      .WillOnce(Invoke([](int, void *buffer, size_t, int) {
        char *buf = (char*) buffer;
        strncpy(buf, "st", 2);
        return 2;
      }));
  EXPECT_CALL(*system, Recv(13, _, 4, 0))
      .WillOnce(Invoke([](int, void *buffer, size_t, int) {
        char *buf = (char*) buffer;
        strncpy(buf, "ing", 4);
        return 4;
      }));
  EXPECT_CALL(*system, Poll(_, 1, detail::TimeoutSeconds * 1000))
      .WillOnce(Return(1))
      .WillOnce(Return(1))
      .WillOnce(Return(1))
      .WillOnce(Return(1));

  NetworkPtr network = Network::Create(system);

  NetworkMessage nm = network->RecvMessage(13);
  ASSERT_EQ(8, nm.size());
  EXPECT_STREQ(nm.data(), "testing");
}

TEST_F(NetworkTest, RecvMessageWhenFirstMessageTimeout) {
  EXPECT_CALL(*system, Poll(_, 1, detail::TimeoutSeconds * 1000)).WillOnce(Return(0));

  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->RecvMessage(13), exception::detail::TimeoutException);
}

TEST_F(NetworkTest, RecvMessageWhenSecondMessageTimeout) {
  EXPECT_CALL(*system, Recv(13, _, 1, 0))
      .WillOnce(Invoke([](int, void *buffer, size_t, int) {
        char *buf = (char*) buffer;
        buf[0] = 5;
        return 1;
      }));
  EXPECT_CALL(*system, Recv(13, _, 5, 0))
      .WillOnce(Invoke([](int, void *buffer, size_t, int) {
        char *buf = (char*) buffer;
        strncpy(buf, "te", 2);
        return 3;
      }));
  EXPECT_CALL(*system, Poll(_, 1, detail::TimeoutSeconds * 1000))
      .WillOnce(Return(1))
      .WillOnce(Return(1))
      .WillOnce(Return(0));

  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->RecvMessage(13), exception::detail::TimeoutException);
}

TEST_F(NetworkTest, RecvMessageOneLongMessage) {
  EXPECT_CALL(*system, Recv(13, _, 1, 0)).WillOnce(Invoke([](int, void *buffer, size_t, int) {
    unsigned char *buf = (unsigned char*) buffer;
    buf[0] = (unsigned char) (255);
    // copy buffer content
    return 256;
  }));
  EXPECT_CALL(*system, Poll(_, 1, detail::TimeoutSeconds * 1000)).WillOnce(Return(1));

  NetworkPtr network = Network::Create(system);

  NetworkMessage nm = network->RecvMessage(13);
  ASSERT_EQ(255, nm.size());
}

TEST_F(NetworkTest, SendMessageOnePart) {
  EXPECT_CALL(*system, Send(13, _, 1, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const unsigned char *buf = (const unsigned char*) buffer;
    int message_length = buf[0];
    EXPECT_EQ(5, message_length);
    return 1;
  }));
  EXPECT_CALL(*system, Send(13, _, 5, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const char *buf = (const char*) buffer;
    EXPECT_STREQ("test", buf);
    return 5;
  }));

  NetworkMessage message{'t', 'e', 's', 't', '\0'};
  NetworkPtr network = Network::Create(system);

  network->SendMessage(13, message);
}

TEST_F(NetworkTest, SendMessageOnePartWithTwoMessageLength) {
  EXPECT_CALL(*system, Send(13, _, 1, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const unsigned char *buf = (const unsigned char*) buffer;
    int message_length = buf[0];
    EXPECT_EQ(5, message_length);
    return 0;
  }))
  .WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const unsigned char *buf = (const unsigned char*) buffer;
    int message_length = buf[0];
    EXPECT_EQ(5, message_length);
    return 1;
  }));
  EXPECT_CALL(*system, Send(13, _, 5, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const char *buf = (const char*) buffer;
    EXPECT_STREQ("test", buf);
    return 5;
  }));

  NetworkMessage message{'t', 'e', 's', 't', '\0'};
  NetworkPtr network = Network::Create(system);

  network->SendMessage(13, message);
}

TEST_F(NetworkTest, SendMessageTwoSendCalls) {
  EXPECT_CALL(*system, Send(13, _, 1, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const unsigned char *buf = (const unsigned char*) buffer;
    int message_length = buf[0];
    EXPECT_EQ(5, message_length);
    return 1;
  }));
  EXPECT_CALL(*system, Send(13, _, 5, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const char *buf = (const char*) buffer;
    EXPECT_EQ('t', buf[0]);
    EXPECT_EQ('e', buf[1]);
    return 2;
  }));
  EXPECT_CALL(*system, Send(13, _, 3, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const char *buf = (const char*) buffer;
    EXPECT_EQ('s', buf[0]);
    EXPECT_EQ('t', buf[1]);
    EXPECT_EQ('\0', buf[2]);
    return 3;
  }));

  NetworkMessage message{'t', 'e', 's', 't', '\0'};
  NetworkPtr network = Network::Create(system);

  network->SendMessage(13, message);
}

TEST_F(NetworkTest, SendMessageWhenMessageIsTooLong) {
  NetworkMessage message;
  message.resize(256);
  NetworkPtr network = Network::Create(system);

  EXPECT_THROW(network->SendMessage(13, message), exception::detail::MessageTooLongException);
}

TEST_F(NetworkTest, SendTextWhenTextIsShort) {
  EXPECT_CALL(*system, Send(11, _, 1, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const unsigned char *buf = (const unsigned char*) buffer;
    int message_length = buf[0];
    EXPECT_EQ(13, message_length);
    return 1;
  }));
  EXPECT_CALL(*system, Send(11, _, 13, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const char *buf = (const char*) buffer;
    char tmp[15] = {0};
    (void) strncpy(tmp, buf, 13);
    EXPECT_STREQ("Lexample text", tmp);
    return 13;
  }));

  std::string text = "example text";

  NetworkPtr network = Network::Create(system);

  network->SendText(11, text);
}

TEST_F(NetworkTest, SendTextWhenTextIsLongAndFitsInOneMessage) {
  std::string text;
  for (int i = 0; i < detail::MaxMessageLength - 1; i++)
    text += "a";
  ASSERT_EQ(254, text.length());
  EXPECT_CALL(*system, Send(11, _, 1, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const unsigned char *buf = (const unsigned char*) buffer;
    int message_length = buf[0];
    EXPECT_EQ(255, message_length);
    return 1;
  }));
  EXPECT_CALL(*system, Send(11, _, 255, 0)).WillOnce(Invoke([&text](int, const void *buffer, size_t, int) {
    const char *buf = (const char*) buffer;
    char tmp[256] = {0};
    (void) strncpy(tmp, buf, 255);
    EXPECT_EQ('L', tmp[0]);
    EXPECT_STREQ(text.c_str(), tmp + 1);
    return 255;
  }));

  NetworkPtr network = Network::Create(system);

  network->SendText(11, text);
}

TEST_F(NetworkTest, SendTextWhenTextIsLongAndFitsInTwoMessages) {
  std::string text;
  for (int i = 0; i < detail::MaxMessageLength - 1; i++)
    text += "a";
  text += "b";

  InSequence s;
  ASSERT_EQ(255, text.length());
  EXPECT_CALL(*system, Send(11, _, 1, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const unsigned char *buf = (const unsigned char*) buffer;
    int message_length = buf[0];
    EXPECT_EQ(255, message_length);
    return 1;
  }));
  EXPECT_CALL(*system, Send(11, _, 255, 0)).WillOnce(Invoke([&text](int, const void *buffer, size_t, int) {
    const char *buf = (const char*) buffer;
    char tmp[256] = {0};
    (void) strncpy(tmp, buf, 255);
    EXPECT_EQ('M', tmp[0]);
    EXPECT_EQ('a', tmp[254]);
    return 255;
  }));
  EXPECT_CALL(*system, Send(11, _, 1, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const unsigned char *buf = (const unsigned char*) buffer;
    int message_length = buf[0];
    EXPECT_EQ(2, message_length);
    return 1;
  }));
  EXPECT_CALL(*system, Send(11, _, 2, 0)).WillOnce(Invoke([&text](int, const void *buffer, size_t, int) {
    const char *buf = (const char*) buffer;
    char tmp[256] = {0};
    (void) strncpy(tmp, buf, 255);
    EXPECT_EQ('L', tmp[0]);
    EXPECT_EQ('b', tmp[1]);
    return 2;
  }));

  NetworkPtr network = Network::Create(system);

  network->SendText(11, text);
}

TEST_F(NetworkTest, SendTextWhenTextIsLongAndFitsInTwoMaximumSizeMessages) {
  std::string text;
  for (int i = 0; i < detail::MaxMessageLength - 1; i++)
    text += "a";
  for (int i = 0; i < detail::MaxMessageLength - 1; i++)
    text += "b";

  InSequence s;
  ASSERT_EQ(508, text.length());
  EXPECT_CALL(*system, Send(11, _, 1, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const unsigned char *buf = (const unsigned char*) buffer;
    int message_length = buf[0];
    EXPECT_EQ(255, message_length);
    return 1;
  }));
  EXPECT_CALL(*system, Send(11, _, 255, 0)).WillOnce(Invoke([&text](int, const void *buffer, size_t, int) {
    const char *buf = (const char*) buffer;
    char tmp[256] = {0};
    (void) strncpy(tmp, buf, 255);
    EXPECT_EQ('M', tmp[0]);
    EXPECT_EQ('a', tmp[254]);
    return 255;
  }));
  EXPECT_CALL(*system, Send(11, _, 1, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const unsigned char *buf = (const unsigned char*) buffer;
    int message_length = buf[0];
    EXPECT_EQ(255, message_length);
    return 1;
  }));
  EXPECT_CALL(*system, Send(11, _, 255, 0)).WillOnce(Invoke([&text](int, const void *buffer, size_t, int) {
    const char *buf = (const char*) buffer;
    char tmp[256] = {0};
    (void) strncpy(tmp, buf, 255);
    EXPECT_EQ('L', tmp[0]);
    EXPECT_EQ('b', tmp[1]);
    EXPECT_EQ('b', tmp[254]);
    return 255;
  }));

  NetworkPtr network = Network::Create(system);

  network->SendText(11, text);
}

TEST_F(NetworkTest, SendTextWhenTextIsLongAndFitsInThreeMessages) {
  std::string text;
  for (int i = 0; i < detail::MaxMessageLength - 1; i++)
    text += "a";
  for (int i = 0; i < detail::MaxMessageLength - 1; i++)
    text += "b";
  text += "c";

  InSequence s;
  ASSERT_EQ(509, text.length());
  EXPECT_CALL(*system, Send(11, _, 1, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const unsigned char *buf = (const unsigned char*) buffer;
    int message_length = buf[0];
    EXPECT_EQ(255, message_length);
    return 1;
  }));
  EXPECT_CALL(*system, Send(11, _, 255, 0)).WillOnce(Invoke([&text](int, const void *buffer, size_t, int) {
    const char *buf = (const char*) buffer;
    char tmp[256] = {0};
    (void) strncpy(tmp, buf, 255);
    EXPECT_EQ('M', tmp[0]);
    EXPECT_EQ('a', tmp[254]);
    return 255;
  }));
  EXPECT_CALL(*system, Send(11, _, 1, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const unsigned char *buf = (const unsigned char*) buffer;
    int message_length = buf[0];
    EXPECT_EQ(255, message_length);
    return 1;
  }));
  EXPECT_CALL(*system, Send(11, _, 255, 0)).WillOnce(Invoke([&text](int, const void *buffer, size_t, int) {
    const char *buf = (const char*) buffer;
    char tmp[256] = {0};
    (void) strncpy(tmp, buf, 255);
    EXPECT_EQ('M', tmp[0]);
    EXPECT_EQ('b', tmp[1]);
    EXPECT_EQ('b', tmp[254]);
    return 255;
  }));
  EXPECT_CALL(*system, Send(11, _, 1, 0)).WillOnce(Invoke([](int, const void *buffer, size_t, int) {
    const unsigned char *buf = (const unsigned char*) buffer;
    int message_length = buf[0];
    EXPECT_EQ(2, message_length);
    return 1;
  }));
  EXPECT_CALL(*system, Send(11, _, 2, 0)).WillOnce(Invoke([&text](int, const void *buffer, size_t, int) {
    const char *buf = (const char*) buffer;
    char tmp[256] = {0};
    (void) strncpy(tmp, buf, 255);
    EXPECT_EQ('L', tmp[0]);
    EXPECT_EQ('c', tmp[1]);
    return 2;
  }));

  NetworkPtr network = Network::Create(system);

  network->SendText(11, text);
}

TEST_F(NetworkTest, ReceiveTextWhenTextIsShort) {
  EXPECT_CALL(*system, Recv(11, _, 1, 0)).WillOnce(Invoke([](int, void *buffer, size_t, int) {
    unsigned char *buf = (unsigned char*) buffer;
    buf[0] = (unsigned char) (13);
    return 1;
  }));
  EXPECT_CALL(*system, Recv(11, _, 13, 0)).WillOnce(Invoke([](int, void *buffer, size_t, int) {
    const char *text = "Lexample text";
    (void) strncpy((char*) buffer, text, 13);
    return 13;
  }));
  EXPECT_CALL(*system, Poll(_, 1, detail::TimeoutSeconds * 1000))
      .WillRepeatedly(Return(1));

  NetworkPtr network = Network::Create(system);

  std::string text = network->ReceiveText(11);
  EXPECT_EQ("example text", text);
}

TEST_F(NetworkTest, ReceiveTextWhenReceivedTwoMessages) {
  {
    InSequence s;

    EXPECT_CALL(*system, Recv(11, _, 1, 0)).WillOnce(Invoke([](int, void *buffer, size_t, int) {
      unsigned char *buf = (unsigned char*) buffer;
      buf[0] = (unsigned char) (13);
      return 1;
    }));
    EXPECT_CALL(*system, Recv(11, _, 13, 0)).WillOnce(Invoke([](int, void *buffer, size_t, int) {
      const char *text = "Mexample text";
      (void) strncpy((char*) buffer, text, 13);
      return 13;
    }));
    EXPECT_CALL(*system, Recv(11, _, 1, 0)).WillOnce(Invoke([](int, void *buffer, size_t, int) {
      unsigned char *buf = (unsigned char*) buffer;
      buf[0] = (unsigned char) (13);
      return 1;
    }));
    EXPECT_CALL(*system, Recv(11, _, 13, 0)).WillOnce(Invoke([](int, void *buffer, size_t, int) {
      const char *text = "Lnext text";
      (void) strncpy((char*) buffer, text, 13);
      return 13;
    }));
  }
  EXPECT_CALL(*system, Poll(_, 1, detail::TimeoutSeconds * 1000))
      .WillRepeatedly(Return(1));

  NetworkPtr network = Network::Create(system);

  std::string text = network->ReceiveText(11);
  EXPECT_EQ("example textnext text", text);
}
