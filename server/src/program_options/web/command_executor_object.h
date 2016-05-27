#ifndef SRC_PROGRAM_OPTIONS_WEB_COMMAND_EXECUTOR_OBJECT_H
#define SRC_PROGRAM_OPTIONS_WEB_COMMAND_EXECUTOR_OBJECT_H

#include <memory>

#include "src/program_options/type/options.h"
#include "src/web/type/command_executor_object_interface.h"

namespace program_options
{

namespace web
{

class CommandExecutorObject;
typedef std::shared_ptr<CommandExecutorObject> CommandExecutorObjectPtr;

class CommandExecutorObject : public ::web::type::CommandExecutorObjectInterface {
 public:
  virtual ~CommandExecutorObject() = default;

  static CommandExecutorObjectPtr Create(const type::Options &options);

  std::string Execute(std::string command);

  bool IsCommandSupported(std::string command);

 private:
  typedef std::string Command;
  const type::Options &options_;

  CommandExecutorObject(const type::Options &options);
  std::string GetServerProcessPid() const;
  std::string GetServerProcessPidfile() const;
};

}

}

#endif /* SRC_PROGRAM_OPTIONS_WEB_COMMAND_EXECUTOR_OBJECT_H */
