#ifndef SIMPLE_COMMAND_OBJECT_H
#define SIMPLE_COMMAND_OBJECT_H

#include <gmock/gmock.h>

#include "src/web/detail/command_executor_object.h"

namespace mock
{

namespace web
{

class SimpleCommandObject : public ::web::detail::CommandExecutorObject {
 public:
  MOCK_METHOD1(Execute, std::string(std::string command));
  MOCK_METHOD1(IsCommandSupported, bool(std::string command));
};

}

}

#endif /* SIMPLE_COMMAND_OBJECT_H */
