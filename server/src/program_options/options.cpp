#include "options.h"

namespace program_options
{

Options::Options()
  : run_as_user_("root"),
    pidfile_path_("/var/run/patlms/dbus.pid"),
    dbus_address_("127.0.0.2"),
    dbus_port_(1032),
    dbus_family_("ipv4"),
    show_help_message_(false)
{
}
  
const std::string& Options::GetRunAsUser() const
{
  return run_as_user_;
}

const std::string& Options::GetPidfilePath() const
{
  return pidfile_path_;
}

const std::string& Options::GetDbusAddress() const
{
  return dbus_address_;
}

const unsigned& Options::GetDbusPort() const
{
  return dbus_port_;
}

const std::string& Options::GetDbusFamily() const
{
  return dbus_family_;
}

void Options::SetRunAsUser(const std::string &username)
{
  run_as_user_ = username;
}
  
void Options::SetPidfilePath(const std::string &path)
{
  pidfile_path_ = path;
}

void Options::SetDbusAddress(const std::string &address)
{
  dbus_address_ = address;
}
  
void Options::SetDbusPort(const unsigned &port)
{
  dbus_port_ = port;
}
  
void Options::SetDbusFamily(const std::string &family)
{
  dbus_family_ = family;
}

void Options::SetShowHelpMessage(bool show)
{
  show_help_message_ = show;
}
  
}
