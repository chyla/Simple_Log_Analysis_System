#include "program_options_command_executor_object.h"

#include <json/json.hpp>
#include <unistd.h>

using namespace std;
using namespace nlohmann;

namespace program_options
{

ProgramOptionsCommandExecutorObjectPtr ProgramOptionsCommandExecutorObject::Create(const Options &options) {
  auto p = ProgramOptionsCommandExecutorObjectPtr(new ProgramOptionsCommandExecutorObject(options));
  return p;
}

std::string ProgramOptionsCommandExecutorObject::Execute(std::string json_command) {
  auto json_object = json::parse(json_command);
  Command command = json_object["command"];
  string result = "{ \"status\" : \"unknown_command\" }";

  if (command == "get_server_process_pid") {
    result = GetServerProcessPid();
  }
  else if (command == "get_server_process_pidfile") {
    result = GetServerProcessPidfile();
  }

  return result;
}

bool ProgramOptionsCommandExecutorObject::IsCommandSupported(std::string command) {
  return (command == "get_server_process_pid")
      || (command == "get_server_process_pidfile");
}

ProgramOptionsCommandExecutorObject::ProgramOptionsCommandExecutorObject(const Options &options)
: options_(options) {
}

std::string ProgramOptionsCommandExecutorObject::GetServerProcessPid() const {
  json j;
  j["status"] = "ok";
  j["result"] = getpid();
  return j.dump();
}

std::string ProgramOptionsCommandExecutorObject::GetServerProcessPidfile() const {
  json j;
  j["status"] = "ok";
  j["result"] = options_.GetPidfilePath();
  return j.dump();
}

}
