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

  inline static const std::string GetUnknownCommandErrorJson();
  inline static const std::string GetInvalidArgumentErrorJson();
};

inline const std::string CommandExecutorObject::GetUnknownCommandErrorJson() {
  return "{ \"status\" : \"error\", \"message\" : \"Unknown command\" }";
}

inline const std::string CommandExecutorObject::GetInvalidArgumentErrorJson() {
  return "{ \"status\" : \"error\", \"message\" : \"Invalid argument\" }";
}

typedef std::shared_ptr<CommandExecutorObject> CommandExecutorObjectPtr;

}

}

#endif /* COMMAND_EXECUTOR_OBJECT_H */
