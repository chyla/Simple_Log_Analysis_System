#pragma once

#include <gmock/gmock.h>

#include "src/web/type/command_executor_object_interface.h"

namespace mock
{

namespace web
{

class SimpleCommandObject : public ::web::type::CommandExecutorObjectInterface {
 public:
  MOCK_METHOD1(Execute, const ::web::type::JsonMessage(const ::web::type::JsonMessage &message));
  MOCK_METHOD1(IsCommandSupported, bool(const ::web::type::Command &command));
};

}

}
