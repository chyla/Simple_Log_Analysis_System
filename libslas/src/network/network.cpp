#include <slas/network/network.h>
#include <slas/network/detail/const_values.h>
#include <slas/network/detail/system.h>
#include <slas/network/exception/detail/cant_open_socket_exception.h>
#include <slas/network/exception/detail/bad_address_exception.h>
#include <slas/network/exception/detail/close_exception.h>
#include <slas/network/exception/detail/poll_exception.h>
#include <slas/network/exception/detail/accept_exception.h>
#include <slas/network/exception/detail/recv_exception.h>
#include <slas/network/exception/detail/timeout_exception.h>
#include <slas/network/exception/detail/message_too_long_exception.h>
#include <slas/network/exception/detail/connect_exception.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <poll.h>

#include <boost/log/trivial.hpp>
#include <cstring>
#include <regex>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <cerrno>

using namespace std;

namespace network
{

Network::~Network() {
}

NetworkPtr Network::Create() {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::Create: Function call";
  detail::SystemInterfacePtr system(new detail::System());
  return Create(system);
}

NetworkPtr Network::Create(detail::SystemInterfacePtr system) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::Create(system): Function call";
  NetworkPtr network_ptr(new Network(system));
  return network_ptr;
}

int Network::Socket(int domain) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::Socket: Function call with (domain=" << domain << ")";
  int socket_fd = system_->Socket(domain, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    BOOST_LOG_TRIVIAL(error) << "libpatlms::network::Network::Socket: Error: " << strerror(errno);
    throw exception::detail::CantOpenSocketException();
  }

  return socket_fd;
}

int Network::OpenUnixSocket(const string &filesystem_path) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::OpenUnixSocket: Function call with (filesystem_path=" << filesystem_path << ")";
  if (filesystem_path.length() > 90) {
    BOOST_LOG_TRIVIAL(error) << "libpatlms::network::Network::OpenUnixSocket: Error: Address too long";
    throw exception::detail::BadAddressException();
  }

  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  addr.sun_path[90] = '\0';
  strncpy(addr.sun_path, filesystem_path.c_str(), 91);

  return OpenSocket(PF_UNIX, (struct sockaddr *) &addr, sizeof (struct sockaddr_un));
}

int Network::OpenIpv4Socket(const string &address, int port) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::OpenIpv4Socket: Function call with (address=" << address << "; port=" << port << ")";
  regex pattern("\\d{1,4}\\.\\d{1,4}\\.\\d{1,4}\\.\\d{1,4}");

  if (!regex_match(address, pattern)) {
    BOOST_LOG_TRIVIAL(error) << "libpatlms::network::Network::OpenIpv4Socket: Error: ipv4 address is not correct";
    throw exception::detail::BadAddressException();
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(address.c_str());
  memset(addr.sin_zero, 0, sizeof (addr.sin_zero));

  return OpenSocket(PF_INET, (struct sockaddr *) &addr, sizeof (struct sockaddr_in));
}

void Network::ConnectUnix(int socket, const std::string &filesystem_path) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::ConnectUnix: Function call with (socket=" << socket << "; filesystem_path=" << filesystem_path << ")";
  if (filesystem_path.length() > 90) {
    BOOST_LOG_TRIVIAL(error) << "libpatlms::network::Network::ConnectUnix: Error: Path too long";
    throw exception::detail::BadAddressException();
  }

  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  addr.sun_path[90] = '\0';
  strncpy(addr.sun_path, filesystem_path.c_str(), 91);

  int ret = system_->Connect(socket, (struct sockaddr *) &addr, sizeof (struct sockaddr_un));
  if (ret < 0) {
    BOOST_LOG_TRIVIAL(error) << "libpatlms::network::Network::ConnectUnix: Error: " << strerror(errno);
    throw exception::detail::ConnectException();
  }

  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::ConnectUnix: Done";
}

void Network::Close(int socket) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::Close: Function call with (socket=" << socket << ")";
  int ret = system_->Close(socket);
  if (ret < 0) {
    BOOST_LOG_TRIVIAL(error) << "libpatlms::network::Network::Close: Error: " << strerror(errno);
    throw exception::detail::CloseException();
  }
}

void Network::SendText(int socket, const string &text) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::SendText: Function call with (socket=" << socket << "; text=" << text << ")";
  const char *ctext = text.c_str();
  NetworkMessage message;
  char *data = nullptr;
  char msg_type = 0;

  int to_copy = 0;
  for (size_t i = 0; i < text.length(); i += (detail::MaxMessageLength - 1)) {
    if (i + detail::MaxMessageLength <= text.length()) {
      to_copy = detail::MaxMessageLength - 1;
      msg_type = 'M';
    }
    else {
      to_copy = text.length() - i;
      msg_type = 'L';
    }

    message.resize(to_copy + 1);
    data = message.data();

    data[0] = msg_type;
    (void) strncpy(data + 1, ctext + i, to_copy);

    SendMessage(socket, message);
  }

  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::SendText: Done";
}

const string Network::ReceiveText(int socket) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::ReceiveText: Function call with (socket=" << socket << ")";
  NetworkMessage message;

  receive_text_buffer.clear();

  do {
    message = RecvMessage(socket);

    receive_text_buffer.reserve(receive_text_buffer.size() + message.size() + 1);
    copy(message.begin() + 1, message.end(), back_inserter(receive_text_buffer));

  }
  while (message.at(0) != 'L');

  receive_text_buffer.push_back('\0');

  string received(receive_text_buffer.data());
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::ReceiveText: Received: " << received << ")";
  return received;
}

NetworkMessage Network::RecvMessage(int socket) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::RecvMessage: Function call with (socket=" << socket << ")";
  WaitStatus wait_status;
  unsigned message_length = 0, total = 0, left_to_receive = 0;
  int received = 0;
  NetworkMessage message;

  wait_status = WaitForData(socket, detail::TimeoutSeconds);
  if (wait_status != WaitStatus::NEW_DATA) {
    BOOST_LOG_TRIVIAL(error) << "libpatlms::network::Network::RecvMessage: Timeout";
    throw exception::detail::TimeoutException();
  }

  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::RecvMessage: Receiving first part";
  received = Recv(socket, buffer, 1);
  total = received;
  message_length = static_cast<unsigned char> (buffer[0]);
  message.resize(message_length);

  (void) memcpy(message.data(), buffer + 1, received - 1);

  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::RecvMessage: Receiving message";
  while (total < message_length) {
    left_to_receive = message_length + 1 - total;

    wait_status = WaitForData(socket, detail::TimeoutSeconds);
    if (wait_status != WaitStatus::NEW_DATA) {
      BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::RecvMessage: Timeout";
      throw exception::detail::TimeoutException();
    }

    received = Recv(socket, buffer, (left_to_receive > detail::BufferLength) ? detail::BufferLength : left_to_receive);
    (void) memcpy(message.data() + total - 1, buffer, received);
    total += received;
  }

  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::RecvMessage: Received";
  return message;
}

void Network::SendMessage(int socket, const NetworkMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::SendMessage: Function call with (socket=" << socket << "; message)";
  int sent = 0, left = 0;

  if (message.size() > detail::MaxMessageLength) {
    BOOST_LOG_TRIVIAL(error) << "libpatlms::network::Network::SendMessage: Message too long (" << message.size() << ")";
    throw exception::detail::MessageTooLongException();
  }

  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::SendMessage: Sending first part";
  unsigned char message_length = static_cast<unsigned char> (message.size());
  do {
    sent = Send(socket, &message_length, 1);
  }
  while (sent == 0);

  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::SendMessage: Sending message";
  const char *buffer = message.data();
  sent = 0;
  left = message_length - sent;
  while (left > 0) {
    sent += Send(socket, buffer + sent, left);
    left = message_length - sent;
  }

  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::SendMessage: Done";
}

WaitStatus Network::WaitForData(int socket, int timeout_seconds) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::WaitForData: Function call with (socket=" << socket << "; timeout_seconds=" << timeout_seconds << ")";
  struct pollfd descriptors = {socket, POLLIN, 0};
  int ret = system_->Poll(&descriptors, 1, timeout_seconds * 1000);

  if (ret == 0) {
    BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::WaitForData: Timeout";
    return WaitStatus::TIMEOUT;
  }
  else if (ret > 0) {
    BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::WaitForData: New data";
    return WaitStatus::NEW_DATA;
  }
  else {
    BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::WaitForData: Poll error: " << strerror(errno);
    throw exception::detail::PollException();
  }
}

ConnectionData Network::Accept(int socket) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::Accept: Function call with (socket=" << socket << ")";
  ConnectionData data;
  data.socket = system_->Accept(socket, (struct sockaddr *) &data.addr, &data.addr_size);
  if (data.socket < 0) {
    BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::Accept: Accept error: " << strerror(errno);
    throw exception::detail::AcceptException();
  }

  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::Accept: Done";
  return data;
}

Network::Network(detail::SystemInterfacePtr system)
: system_(system) {
}

int Network::OpenSocket(int domain, struct sockaddr *saddr, int saddr_size) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::OpenSocket: Function call with (domain=" << domain << "; sockaddr*; saddr_size=" << saddr_size << ")";
  int ret, socket_fd;

  socket_fd = Socket(domain);

  int optval = 1;
  ret = system_->Setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
  if (ret < 0) {
    BOOST_LOG_TRIVIAL(error) << "libpatlms::network::Network::OpenSocket: Setsockopt error: " << strerror(errno);
    throw exception::detail::CantOpenSocketException();
  }

  ret = system_->Bind(socket_fd, saddr, saddr_size);
  if (ret < 0) {
    BOOST_LOG_TRIVIAL(error) << "libpatlms::network::Network::OpenSocket: Bind error: " << strerror(errno);
    throw exception::detail::CantOpenSocketException();
  }

  ret = system_->Listen(socket_fd, 20);
  if (ret < 0) {
    BOOST_LOG_TRIVIAL(error) << "libpatlms::network::Network::OpenSocket: Listen error: " << strerror(errno);
    throw exception::detail::CantOpenSocketException();
  }

  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::OpenSocket: Done";
  return socket_fd;
}

size_t Network::Recv(int socket, void *buffer, size_t length) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::Recv: Functino call with (socket=" << socket << "; buffer*; length=" << length << ")";
  int received = system_->Recv(socket, buffer, length, 0);
  if (received == 0 || received < 0) {
    BOOST_LOG_TRIVIAL(error) << "libpatlms::network::Network::Recv: Recv error: " << strerror(errno);
    throw exception::detail::RecvException();
  }

  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::Recv: Done";
  return received;
}

size_t Network::Send(int socket, const void *buffer, size_t length) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::Send: Functino call with (socket=" << socket << "; buffer*; length=" << length << ")";
  int sent = system_->Send(socket, buffer, length, 0);
  if (sent < 0) {
    BOOST_LOG_TRIVIAL(error) << "libpatlms::network::Network::Recv: Recv error: " << strerror(errno);
    throw exception::detail::RecvException();
  }

  BOOST_LOG_TRIVIAL(debug) << "libpatlms::network::Network::Send: Done";
  return sent;
}

}
