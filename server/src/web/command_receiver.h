#pragma once

#include <memory>

#include <patlms/network/detail/network_interface.h>

#include "detail/command_receiver_interface.h"
#include "detail/command_executor_interface.h"

namespace web
{

class CommandReceiver;
typedef std::shared_ptr<CommandReceiver> CommandReceiverPtr;

class CommandReceiver : public detail::CommandReceiverInterface {
 public:
  virtual ~CommandReceiver() = default;

  static CommandReceiverPtr Create(detail::CommandExecutorInterfacePtr command_executor);
  static CommandReceiverPtr Create(detail::CommandExecutorInterfacePtr command_executor,
                                   network::detail::NetworkInterfacePtr network);

  void OpenPort(const std::string &address, const int port) override;
  void ClosePort() override;
  bool IsPortOpen() const override;

  void StartListen() override;
  void StopListen() override;
  bool IsListen() const override;

 protected:
  CommandReceiver(detail::CommandExecutorInterfacePtr command_executor,
                  network::detail::NetworkInterfacePtr network);

 private:
  detail::CommandExecutorInterfacePtr command_executor_;
  network::detail::NetworkInterfacePtr network_;

  int socket_;
  bool is_listen_;

  void ReadAndExecuteCommand(int client_socket);
};

}
