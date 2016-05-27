#ifndef SRC_WEB_COMMAND_EXECUTOR_H
#define SRC_WEB_COMMAND_EXECUTOR_H

#include "detail/command_executor_interface.h"

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

  void RegisterCommandObject(type::CommandExecutorObjectInterfacePtr object) override;

 private:
  CommandExecutor();

  std::set<type::CommandExecutorObjectInterfacePtr> command_objects_;
};

}

#endif /* SRC_WEB_COMMAND_EXECUTOR_H */
