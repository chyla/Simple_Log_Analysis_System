#ifndef COMMAND_EXECUTOR_OBJECT_H
#define COMMAND_EXECUTOR_OBJECT_H

#include <string>
#include <memory>

namespace web
{

namespace detail
{

class CommandExecutorObject {
 public:
  virtual ~CommandExecutorObject() = default;

  virtual std::string Execute(std::string command) = 0;

  virtual bool IsCommandSupported(std::string command) = 0;
};

typedef std::shared_ptr<CommandExecutorObject> CommandExecutorObjectPtr;

}

}

#endif /* COMMAND_EXECUTOR_OBJECT_H */
