#ifndef SRC_WEB_COMMAND_EXECUTOR_H
#define SRC_WEB_COMMAND_EXECUTOR_H

#include "detail/command_executor_interface.h"

#include <set>
#include <memory>

#include "type/json_message.h"

namespace web
{

class CommandExecutor;
typedef std::shared_ptr<CommandExecutor> CommandExecutorPtr;

class CommandExecutor : public detail::CommandExecutorInterface {
 public:
  virtual ~CommandExecutor() = default;

  static CommandExecutorPtr Create();

  const type::JsonMessage Execute(const type::JsonMessage &message) override;

  void RegisterCommandObject(type::CommandExecutorObjectInterfacePtr object) override;

 private:
  CommandExecutor();

  std::set<type::CommandExecutorObjectInterfacePtr> command_objects_;

  ::web::type::JsonMessage GetErrorMessage(const std::string &what) const;
};

}

#endif /* SRC_WEB_COMMAND_EXECUTOR_H */
