#include "command_executor.h"

#include <boost/log/trivial.hpp>
#include <json/json.hpp>

using namespace std;
using namespace nlohmann;

namespace web
{

CommandExecutorPtr CommandExecutor::Create() {
  BOOST_LOG_TRIVIAL(debug) << "web::CommandExecutor::Create: Function call";
  auto p = CommandExecutorPtr(new CommandExecutor());
  return p;
}

const type::JsonMessage CommandExecutor::Execute(const type::JsonMessage &message) {
  BOOST_LOG_TRIVIAL(debug) << "web::CommandExecutor::Execute: Function call";

  auto json_object = json::parse(message);
  type::Command command = json_object["command"];
  type::JsonMessage result = type::CommandExecutorObjectInterface::GetUnknownCommandErrorJson();

  BOOST_LOG_TRIVIAL(debug) << "web::CommandExecutor::Execute: json_command='" << message << "', command='" << command << "'";

  for (auto ptr : command_objects_) {
    if (ptr->IsCommandSupported(command)) {
      BOOST_LOG_TRIVIAL(debug) << "web::CommandExecutor::Execute: Found object";
      result = ptr->Execute(message);
    }
  }

  BOOST_LOG_TRIVIAL(debug) << "web::CommandExecutor::Execute: Returning: " << result;
  return result;
}

void CommandExecutor::RegisterCommandObject(type::CommandExecutorObjectInterfacePtr object) {
  BOOST_LOG_TRIVIAL(debug) << "web::CommandExecutor::RegisterCommandObject: Function call";
  command_objects_.insert(object);
}

CommandExecutor::CommandExecutor() {
}

}
