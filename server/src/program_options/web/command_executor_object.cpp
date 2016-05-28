#include "command_executor_object.h"

#include <json/json.hpp>
#include <unistd.h>

using namespace std;
using namespace nlohmann;
using namespace program_options::type;

namespace program_options
{

namespace web
{

CommandExecutorObjectPtr CommandExecutorObject::Create(const Options &options) {
  auto p = CommandExecutorObjectPtr(new CommandExecutorObject(options));
  return p;
}

const ::web::type::JsonMessage CommandExecutorObject::Execute(const ::web::type::JsonMessage &message) {
  auto json_object = json::parse(message);
  auto command = json_object["command"];
  auto result = GetUnknownCommandErrorJson();

  if (command == "get_server_process_pid") {
    result = GetServerProcessPid();
  }
  else if (command == "get_server_process_pidfile") {
    result = GetServerProcessPidfile();
  }

  return result;
}

bool CommandExecutorObject::IsCommandSupported(const ::web::type::Command &command) {
  return (command == "get_server_process_pid")
      || (command == "get_server_process_pidfile");
}

CommandExecutorObject::CommandExecutorObject(const Options &options)
: options_(options) {
}

const ::web::type::JsonMessage CommandExecutorObject::GetServerProcessPid() const {
  json j;
  j["status"] = "ok";
  j["result"] = getpid();
  return j.dump();
}

const ::web::type::JsonMessage CommandExecutorObject::GetServerProcessPidfile() const {
  json j;
  j["status"] = "ok";
  j["result"] = options_.GetPidfilePath();
  return j.dump();
}

}

}
