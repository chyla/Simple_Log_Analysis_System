#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <patlms/network/network_message.h>
#include <patlms/network/connection_data.h>
#include <patlms/network/wait_status.h>

#include <string>

namespace network
{

namespace detail
{

class NetworkInterface {
 public:
  virtual ~NetworkInterface();

  virtual int OpenUnixSocket(const std::string &path) = 0;
  virtual int OpenIpv4Socket(const std::string &address, int port) = 0;
  virtual void Close(int socket) = 0;

  virtual void SendText(int socket, const std::string &text) = 0;
  virtual const std::string ReceiveText(int socket) = 0;

  virtual NetworkMessage RecvMessage(int socket) = 0;
  virtual void SendMessage(int socket, const NetworkMessage &message) = 0;

  virtual WaitStatus WaitForData(int socket, int timeout) = 0;

  virtual ConnectionData Accept(int socket) = 0;
};

}

}

#endif /* NETWORK_INTERFACE_H */
