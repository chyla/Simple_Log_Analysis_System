#pragma once

#include <slas/network/detail/network_interface.h>
#include <slas/network/detail/system_interface.h>
#include <slas/network/detail/const_values.h>

namespace network
{
class Network;
typedef std::shared_ptr<Network> NetworkPtr;

class Network : public detail::NetworkInterface {
 public:
  virtual ~Network();

  static NetworkPtr Create();
  static NetworkPtr Create(detail::SystemInterfacePtr system);

  int Socket(int domain = PF_INET) override;

  int OpenUnixSocket(const std::string &path) override;
  int OpenIpv4Socket(const std::string &address, int port) override;
  void ConnectUnix(int socket, const std::string &filesystem_path) override;
  void Close(int socket) override;

  void SendText(int socket, const std::string &text);
  const std::string ReceiveText(int socket);

  NetworkMessage RecvMessage(int socket) override;
  void SendMessage(int socket, const NetworkMessage &message) override;

  WaitStatus WaitForData(int socket, int timeout_seconds) override;

  ConnectionData Accept(int socket) override;

 private:
  Network(detail::SystemInterfacePtr system);

  int OpenSocket(int domain, struct sockaddr *saddr, int saddr_size);

  size_t Recv(int socket, void *buffer, size_t length);
  size_t Send(int socket, const void *buffer, size_t length);

  detail::SystemInterfacePtr system_;
  char buffer[detail::BufferLength];
  std::vector<char> receive_text_buffer;
};

}
