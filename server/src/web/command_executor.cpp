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

std::string CommandExecutor::Execute(std::string json_command) {
  BOOST_LOG_TRIVIAL(debug) << "web::CommandExecutor::Execute: Function call";

  auto json_object = json::parse(json_command);
  string command = json_object["command"];
  string result = "{ \"status\" : \"unknown_command\" }";

  BOOST_LOG_TRIVIAL(debug) << "web::CommandExecutor::Execute: json_command='" << json_command << "', command='" << command << "'";

  for (auto ptr : command_objects_) {
    if (ptr->IsCommandSupported(command)) {
      BOOST_LOG_TRIVIAL(debug) << "web::CommandExecutor::Execute: Found object";
      result = ptr->Execute(json_command);
    }
  }

  BOOST_LOG_TRIVIAL(debug) << "web::CommandExecutor::Execute: Returning: " << result;
  return result;
}

void CommandExecutor::RegisterCommandObject(detail::CommandExecutorObjectPtr object) {
  BOOST_LOG_TRIVIAL(debug) << "web::CommandExecutor::RegisterCommandObject: Function call";
  command_objects_.insert(object);
}

CommandExecutor::CommandExecutor() {
}

}
