#include "command_executor_object.h"

#include <boost/log/trivial.hpp>
#include <json/json.hpp>
#include <algorithm>

using namespace std;
using namespace nlohmann;

namespace apache
{

namespace web
{

CommandExecutorObjectPtr CommandExecutorObject::Create(::database::DatabasePtr database) {
  BOOST_LOG_TRIVIAL(debug) << "apache::web::CommandExecutorObject::Create: Function call";
  auto p = CommandExecutorObjectPtr(new CommandExecutorObject(database));
  return p;
}

std::string CommandExecutorObject::Execute(std::string json_command) {
  BOOST_LOG_TRIVIAL(debug) << "apache::web::CommandExecutorObject::Execute: Function call";
  auto json_object = json::parse(json_command);
  auto command = json_object["command"];
  auto result = GetUnknownCommandErrorJson();

  if (command == "get_apache_agent_names") {
    BOOST_LOG_TRIVIAL(warning) << "apache::web::CommandExecutorObject::Execute: Found 'get_apache_agent_names' command";
    result = GetHostnames();
  }
  else if (command == "get_apache_virtualhosts_names") {
    BOOST_LOG_TRIVIAL(warning) << "apache::web::CommandExecutorObject::Execute: Found 'get_apache_virtualhosts_names' command";

    auto args = json_object["args"];
    if (args.size() != 1) {
      BOOST_LOG_TRIVIAL(warning) << "apache::web::CommandExecutorObject::Execute: get_apache_virtualhosts_names require one argument";
      return GetInvalidArgumentErrorJson();
    }

    auto agent_name = args.at(0);
    result = GetVirtualhostsNames(agent_name);
  }

  return result;
}

bool CommandExecutorObject::IsCommandSupported(std::string command) {
  BOOST_LOG_TRIVIAL(debug) << "apache::web::CommandExecutorObject::IsCommandSupported: Function call";
  return (command == "get_apache_agent_names")
      || (command == "get_apache_virtualhosts_names");
}

CommandExecutorObject::CommandExecutorObject(::database::DatabasePtr database)
: database_(database) {
}

std::string CommandExecutorObject::GetHostnames() {
  BOOST_LOG_TRIVIAL(debug) << "apache::web::CommandExecutorObject::GetHostnames: Function call";

  json j;
  j["status"] = "ok";
  j["result"] = database_->GetApacheAgentNames();

  return j.dump();
}

std::string CommandExecutorObject::GetVirtualhostsNames(const std::string &agent_name) {
  BOOST_LOG_TRIVIAL(debug) << "apache::web::CommandExecutorObject::GetVirtualhostsNames: Function call";

  json j;
  j["status"] = "ok";
  j["result"] = database_->GetApacheVirtualhostNames(agent_name);

  return j.dump();
}

}

}
