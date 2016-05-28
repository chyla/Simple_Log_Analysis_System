#ifndef SRC_WEB_TYPE_COMMAND_EXECUTOR_OBJECT_INTERFACE_H
#define SRC_WEB_TYPE_COMMAND_EXECUTOR_OBJECT_INTERFACE_H

#include <string>
#include <memory>

#include "json_message.h"
#include "command.h"

namespace web
{

namespace type
{

class CommandExecutorObjectInterface {
 public:
  virtual ~CommandExecutorObjectInterface() = default;

  virtual const JsonMessage Execute(const JsonMessage &message) = 0;

  virtual bool IsCommandSupported(const Command &command) = 0;

  inline static const JsonMessage GetUnknownCommandErrorJson();
  inline static const JsonMessage GetInvalidArgumentErrorJson();
};

inline const JsonMessage CommandExecutorObjectInterface::GetUnknownCommandErrorJson() {
  return "{ \"status\" : \"error\", \"message\" : \"Unknown command\" }";
}

inline const JsonMessage CommandExecutorObjectInterface::GetInvalidArgumentErrorJson() {
  return "{ \"status\" : \"error\", \"message\" : \"Invalid argument\" }";
}

typedef std::shared_ptr<CommandExecutorObjectInterface> CommandExecutorObjectInterfacePtr;

}

}

#endif /* SRC_WEB_TYPE_COMMAND_EXECUTOR_OBJECT_INTERFACE_H */
