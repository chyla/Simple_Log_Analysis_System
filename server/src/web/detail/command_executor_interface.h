#ifndef SRC_WEB_DETAIL_COMMAND_EXECUTOR_INTERFACE_H
#define SRC_WEB_DETAIL_COMMAND_EXECUTOR_INTERFACE_H

#include <memory>
#include <string>

#include "command_executor_object.h"

namespace web
{

namespace detail
{

class CommandExecutorInterface {
 public:
  virtual ~CommandExecutorInterface() = default;

  virtual std::string Execute(std::string command) = 0;

  virtual void RegisterCommandObject(CommandExecutorObjectPtr object) = 0;
};

typedef std::shared_ptr<CommandExecutorInterface> CommandExecutorInterfacePtr;

}

}

#endif /* SRC_WEB_DETAIL_COMMAND_EXECUTOR_INTERFACE_H */
