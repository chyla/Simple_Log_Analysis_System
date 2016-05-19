#include "command_executor.h"

#include <json/json.hpp>

using namespace std;
using namespace nlohmann;

namespace web
{

CommandExecutorPtr CommandExecutor::Create() {
  auto p = CommandExecutorPtr(new CommandExecutor());
  return p;
}

std::string CommandExecutor::Execute(std::string json_command) {
  auto json_object = json::parse(json_command);
  string command = json_object["command"];
  string result = "{ \"status\" : \"unknown_command\" }";

  for (auto ptr : command_objects_) {
    if (ptr->IsCommandSupported(command)) {
      result = ptr->Execute(json_command);
    }
  }

  return result;
}

void CommandExecutor::RegisterCommandObject(detail::CommandExecutorObjectPtr object) {
  command_objects_.insert(object);
}

CommandExecutor::CommandExecutor() {
}

}
