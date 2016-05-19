#ifndef PROGRAM_OPTIONS_COMMAND_EXECUTOR_OBJECT_H
#define PROGRAM_OPTIONS_COMMAND_EXECUTOR_OBJECT_H

#include "options.h"
#include "src/web/detail/command_executor_object.h"

#include <memory>

namespace program_options
{

class ProgramOptionsCommandExecutorObject;
typedef std::shared_ptr<ProgramOptionsCommandExecutorObject> ProgramOptionsCommandExecutorObjectPtr;

class ProgramOptionsCommandExecutorObject : public web::detail::CommandExecutorObject {
 public:
  virtual ~ProgramOptionsCommandExecutorObject() = default;
  
  static ProgramOptionsCommandExecutorObjectPtr Create(const Options &options);

  std::string Execute(std::string command);

  bool IsCommandSupported(std::string command);
  
 private:
  typedef std::string Command;
  const Options &options_;
  
  ProgramOptionsCommandExecutorObject(const Options &options);
  std::string GetServerProcessPid() const;
  std::string GetServerProcessPidfile() const;
};

}

#endif /* PROGRAM_OPTIONS_COMMAND_EXECUTOR_OBJECT_H */
