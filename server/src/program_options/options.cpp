#include "options.h"

namespace program_options
{

const Options Options::Create(const std::string &run_as_user,
                              const std::string &pidfile_path,
                              const std::string &logfile_path,
                              const std::string &dbus_address,
                              unsigned dbus_port,
                              const std::string &dbus_family,
                              bool help_message,
                              bool daemon,
                              const std::string &databasefile_path) {
  Options options;
  options.run_as_user_ = run_as_user;
  options.pidfile_path_ = pidfile_path;
  options.logfile_path_ = logfile_path;
  options.dbus_address_ = dbus_address;
  options.dbus_port_ = dbus_port;
  options.dbus_family_ = dbus_family;
  options.show_help_message_ = help_message;
  options.daemon_ = daemon;
  options.databasefile_path_ = databasefile_path;

  return options;
}

const std::string& Options::GetRunAsUser() const {
  return run_as_user_;
}

const std::string& Options::GetPidfilePath() const {
  return pidfile_path_;
}

const std::string& Options::GetLogfilePath() const {
  return logfile_path_;
}

const std::string& Options::GetDbusAddress() const {
  return dbus_address_;
}

const unsigned& Options::GetDbusPort() const {
  return dbus_port_;
}

const std::string& Options::GetDbusFamily() const {
  return dbus_family_;
}

bool Options::IsDaemon() const {
  return daemon_;
}

const std::string& Options::GetDatabasefilePath() const {
  return databasefile_path_;
}

}
