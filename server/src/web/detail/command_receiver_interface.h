#pragma once

#include <memory>

namespace web
{

namespace detail
{

class CommandReceiverInterface {
 public:
  virtual ~CommandReceiverInterface() = default;

  virtual void OpenPort(const std::string &address, const int port) = 0;
  virtual void ClosePort() = 0;
  virtual bool IsPortOpen() const = 0;

  virtual void StartListen() = 0;
  virtual void StopListen() = 0;
  virtual bool IsListen() const = 0;
};

typedef std::shared_ptr<CommandReceiverInterface> CommandReceiverInterfacePtr;

}

}
