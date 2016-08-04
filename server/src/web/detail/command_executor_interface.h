#pragma once

#include <memory>
#include <string>

#include "src/web/type/command_executor_object_interface.h"

namespace web
{

namespace detail
{

class CommandExecutorInterface {
 public:
  virtual ~CommandExecutorInterface() = default;

  virtual const type::JsonMessage Execute(const type::JsonMessage &message) = 0;

  virtual void RegisterCommandObject(type::CommandExecutorObjectInterfacePtr object) = 0;
};

typedef std::shared_ptr<CommandExecutorInterface> CommandExecutorInterfacePtr;

}

}
