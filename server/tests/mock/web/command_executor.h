#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include <gmock/gmock.h>

#include "src/web/detail/command_executor_interface.h"

namespace mock
{

namespace web
{

class CommandExecutor : public ::web::detail::CommandExecutorInterface {
 public:
  MOCK_METHOD1(Execute, std::string(std::string command));
};

}

}

#endif /* COMMAND_EXECUTOR_H */
