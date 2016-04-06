#include "parser.h"

#include <sstream>

using namespace std;
using namespace boost::program_options;


namespace program_options
{

Parser::Parser()
  : cmd_params_(0, nullptr),
    general_options_("General options"),
    help_options_("Help options")
{
  general_options_.add_options()
    ("agent_name", value<string>(), "agent name")
    ("run_as_user", value<string>(), "run as user")
    ("dbus_address", value<string>(), "D-Bus bus address")
    ("dbus_port", value<unsigned>(), "D-Bus bus port")
    ("dbus_family", value<string>(), "D-Bus bus family")
    ("pidfile", value<string>(), "pidfile path");

  help_options_.add_options()
    ("help,h", "print help message and exit");

  all_options_.add(general_options_);
  all_options_.add(help_options_);
}


std::string Parser::GetHelpMessage() const
{
  stringstream stream;
  stream << all_options_;

  return stream.str();
}

void Parser::SetCommandLineOptions(const int &argc, char **argv)
{
  cmd_params_ = std::make_pair(argc, argv);
}

void Parser::SetConfigFilePath(const string& path)
{
  config_file_path_ = path;
}

Options Parser::Parse()
{
  typedef boost::program_options::variables_map variables_map;
  variables_map variables;

  if (cmd_params_.first != 0 && cmd_params_.second != nullptr) {
    store(parse_command_line(cmd_params_.first,
			     cmd_params_.second,
			     all_options_),
	  variables);
  }

  if (!config_file_path_.empty()) {
    const char *config_file = config_file_path_.c_str();
    store(parse_config_file<char>(config_file, general_options_),
	  variables);
  }

  notify(variables);

  Options options;
  options.SetShowHelpMessage(variables.count("help"));

  if (variables.count("agent_name"))
    options.SetAgentName(variables["agent_name"].as<string>());

  if (variables.count("run_as_user"))
    options.SetRunAsUser(variables["run_as_user"].as<string>());

  if (variables.count("pidfile"))
    options.SetPidfilePath(variables["pidfile"].as<string>());

  if (variables.count("dbus_address"))
    options.SetDbusAddress(variables["dbus_address"].as<string>());

  if (variables.count("dbus_port"))
    options.SetDbusPort(variables["dbus_port"].as<unsigned>());

  if (variables.count("dbus_family"))
    options.SetDbusFamily(variables["dbus_family"].as<string>());
  
  return options;
}
  
}
