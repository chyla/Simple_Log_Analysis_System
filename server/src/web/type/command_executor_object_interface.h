#ifndef SRC_WEB_TYPE_COMMAND_EXECUTOR_OBJECT_INTERFACE_H
#define SRC_WEB_TYPE_COMMAND_EXECUTOR_OBJECT_INTERFACE_H

#include <string>
#include <memory>

namespace web
{

namespace type
{

class CommandExecutorObjectInterface {
 public:
  virtual ~CommandExecutorObjectInterface() = default;

  virtual std::string Execute(std::string command) = 0;

  virtual bool IsCommandSupported(std::string command) = 0;

  inline static const std::string GetUnknownCommandErrorJson();
  inline static const std::string GetInvalidArgumentErrorJson();
};

inline const std::string CommandExecutorObjectInterface::GetUnknownCommandErrorJson() {
  return "{ \"status\" : \"error\", \"message\" : \"Unknown command\" }";
}

inline const std::string CommandExecutorObjectInterface::GetInvalidArgumentErrorJson() {
  return "{ \"status\" : \"error\", \"message\" : \"Invalid argument\" }";
}

typedef std::shared_ptr<CommandExecutorObjectInterface> CommandExecutorObjectInterfacePtr;

}

}

#endif /* SRC_WEB_TYPE_COMMAND_EXECUTOR_OBJECT_INTERFACE_H */
