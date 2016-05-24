#include "command_executor_object.h"

#include <boost/log/trivial.hpp>
#include <json/json.hpp>
#include <algorithm>

#include <patlms/type/time.h>

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
  const std::string command = json_object["command"];
  auto result = GetUnknownCommandErrorJson();

  if (command == "get_apache_agent_names") {
    BOOST_LOG_TRIVIAL(info) << "apache::web::CommandExecutorObject::Execute: Found 'get_apache_agent_names' command";
    result = GetHostnames();
  }
  else if (command == "get_apache_virtualhosts_names") {
    BOOST_LOG_TRIVIAL(info) << "apache::web::CommandExecutorObject::Execute: Found 'get_apache_virtualhosts_names' command";

    auto args = json_object["args"];
    if (args.size() != 1) {
      BOOST_LOG_TRIVIAL(warning) << "apache::web::CommandExecutorObject::Execute: get_apache_virtualhosts_names require one argument";
      return GetInvalidArgumentErrorJson();
    }

    auto agent_name = args.at(0);
    result = GetVirtualhostsNames(agent_name);
  }
  else if (command == "get_apache_sessions") {
    BOOST_LOG_TRIVIAL(info) << "apache::web::CommandExecutorObject::Execute: Found 'get_apache_sessions' command";

    auto args = json_object["args"];
    if (args.size() != 4) {
      BOOST_LOG_TRIVIAL(warning) << "apache::web::CommandExecutorObject::Execute: get_apache_sessions require four arguments";
      return GetInvalidArgumentErrorJson();
    }

    result = GetSessions(args.at(0), args.at(1), args.at(2), args.at(3));
  }
  else if (command == "set_apache_sessions_as_anomaly") {
    BOOST_LOG_TRIVIAL(info) << "apache::web::CommandExecutorObject::Execute: Found 'set_apache_sessions_as_anomaly' command";

    auto args = json_object["args"];
    if (args.size() != 2) {
      BOOST_LOG_TRIVIAL(warning) << "apache::web::CommandExecutorObject::Execute: set_apache_sessions_as_anomaly require two arguments";
      return GetInvalidArgumentErrorJson();
    }

    result = SetApacheSessionsAsAnomaly(args.at(0), args.at(1));
  }

  return result;
}

bool CommandExecutorObject::IsCommandSupported(std::string command) {
  BOOST_LOG_TRIVIAL(debug) << "apache::web::CommandExecutorObject::IsCommandSupported: Function call";
  return (command == "get_apache_agent_names")
      || (command == "get_apache_virtualhosts_names")
      || (command == "get_apache_sessions")
      || (command == "set_apache_sessions_as_anomaly");
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

std::string CommandExecutorObject::GetSessions(const std::string &agent_name,
                                               const std::string &virtualhost_name,
                                               const std::string &begin_date,
                                               const std::string &end_date) {
  BOOST_LOG_TRIVIAL(debug) << "apache::web::CommandExecutorObject::GetSessions: Function call";

  auto tbegin = type::Time::FromString(begin_date);
  auto tend = type::Time::FromString(end_date);
  auto count = database_->GetApacheSessionStatisticsCount(agent_name, virtualhost_name,
                                                          tbegin, tend);

  analyzer::ApacheSessions sessions = database_->GetApacheSessionStatistics(agent_name, virtualhost_name,
                                                                            tbegin, tend,
                                                                            count, 0);

  json j, r = json::array();
  for (analyzer::ApacheSessionEntry s : sessions) {
    json t;
    t["id"] = s.id;
    t["agent_name"] = s.agent_name;
    t["virtualhost"] = s.virtualhost;
    t["client_ip"] = s.client_ip;
    t["session_start"] = s.session_start.ToString();
    t["session_length"] = s.session_length;
    t["bandwidth_usage"] = s.bandwidth_usage;
    t["requests_count"] = s.requests_count;
    t["error_percentage"] = s.error_percentage;
    t["useragent"] = s.useragent;
    t["is_anomaly"] = s.is_anomaly;

    r.push_back(t);
  }

  j["status"] = "ok";
  j["result"] = r;

  return j.dump();
}

const std::string CommandExecutorObject::SetApacheSessionsAsAnomaly(const std::vector<long long> &all,
                                                                    const std::vector<long long> &anomalies) {
  BOOST_LOG_TRIVIAL(debug) << "apache::web::CommandExecutorObject::SetApacheSessionsAsAnomaly: Function call";

  database_->SetApacheSessionAsAnomaly(all, anomalies);

  json j;
  j["status"] = "ok";

  return j.dump();
}

}

}
