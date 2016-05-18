#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include "detail/command_executor_interface.h"

namespace web
{

class CommandExecutor : detail::CommandExecutorInterface {
 public:
  virtual ~CommandExecutor() = default;

  std::string Execute(std::string command) override;
};

}

#endif /* COMMAND_EXECUTOR_H */
