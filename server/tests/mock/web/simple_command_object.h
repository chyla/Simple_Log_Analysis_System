#ifndef SIMPLE_COMMAND_OBJECT_H
#define SIMPLE_COMMAND_OBJECT_H

#include <gmock/gmock.h>

#include "src/web/type/command_executor_object_interface.h"

namespace mock
{

namespace web
{

class SimpleCommandObject : public ::web::type::CommandExecutorObjectInterface {
 public:
  MOCK_METHOD1(Execute, std::string(std::string command));
  MOCK_METHOD1(IsCommandSupported, bool(std::string command));
};

}

}

#endif /* SIMPLE_COMMAND_OBJECT_H */
