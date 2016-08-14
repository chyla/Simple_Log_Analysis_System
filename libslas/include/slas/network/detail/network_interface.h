#pragma once

#include <slas/network/network_message.h>
#include <slas/network/connection_data.h>
#include <slas/network/wait_status.h>

#include <string>
#include <memory>
#include <sys/socket.h>

namespace network
{

namespace detail
{

class NetworkInterface {
 public:
  virtual ~NetworkInterface();

  virtual int Socket(int domain = PF_INET) = 0;

  virtual int OpenUnixSocket(const std::string &path) = 0;
  virtual int OpenIpv4Socket(const std::string &address, int port) = 0;
  virtual void ConnectUnix(int socket, const std::string &filesystem_path) = 0;
  virtual void Close(int socket) = 0;

  virtual void SendText(int socket, const std::string &text) = 0;
  virtual const std::string ReceiveText(int socket) = 0;

  virtual NetworkMessage RecvMessage(int socket) = 0;
  virtual void SendMessage(int socket, const NetworkMessage &message) = 0;

  virtual WaitStatus WaitForData(int socket, int timeout) = 0;

  virtual ConnectionData Accept(int socket) = 0;
};

typedef std::shared_ptr<NetworkInterface> NetworkInterfacePtr;

}

}
