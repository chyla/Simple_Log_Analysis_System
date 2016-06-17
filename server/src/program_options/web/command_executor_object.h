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

  const ::web::type::JsonMessage Execute(const ::web::type::JsonMessage &message);

  bool IsCommandSupported(const ::web::type::Command &command);

 private:
  const type::Options &options_;

  CommandExecutorObject(const type::Options &options);
  const ::web::type::JsonMessage GetServerProcessPid() const;
  const ::web::type::JsonMessage GetServerProcessPidfile() const;
  const ::web::type::JsonMessage GetServerVersion() const;
};

}

}

#endif /* SRC_PROGRAM_OPTIONS_WEB_COMMAND_EXECUTOR_OBJECT_H */
