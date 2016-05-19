#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include "detail/command_executor_interface.h"
#include "src/program_options/options.h"

#include <set>
#include <memory>

namespace web
{

class CommandExecutor;
typedef std::shared_ptr<CommandExecutor> CommandExecutorPtr;

class CommandExecutor : public detail::CommandExecutorInterface {
 public:
  virtual ~CommandExecutor() = default;

  static CommandExecutorPtr Create();

  std::string Execute(std::string command) override;

  void RegisterCommandObject(detail::CommandExecutorObjectPtr object) override;

 private:
  CommandExecutor();

  std::set<detail::CommandExecutorObjectPtr> command_objects_;
};

}

#endif /* COMMAND_EXECUTOR_H */
