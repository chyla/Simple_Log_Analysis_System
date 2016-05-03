#ifndef PROGRAM_OPTIONS_OPTIONS_H
#define PROGRAM_OPTIONS_OPTIONS_H

#include <string>


namespace program_options
{

class Options {
 public:
  static const Options Create(const std::string &agent_name,
                       const std::string &run_as_user,
                       const std::string &pidfile_path,
                       const std::string &logfile_path,
                       const std::string &dbus_address,
                       unsigned dbus_port,
                       const std::string &dbus_family,
                       bool help_message);

  const std::string& GetAgentName() const;
  const std::string& GetRunAsUser() const;
  const std::string& GetPidfilePath() const;
  const std::string& GetLogfilePath() const;

  const std::string& GetDbusAddress() const;
  const unsigned& GetDbusPort() const;
  const std::string& GetDbusFamily() const;

  bool IsHelpMessage() const;

 private:
  Options() = default;

  std::string agent_name_;
  std::string run_as_user_;
  std::string pidfile_path_;
  std::string logfile_path_;

  std::string dbus_address_;
  unsigned dbus_port_;
  std::string dbus_family_;

  bool help_message_;
};

}

#endif // PROGRAM_OPTIONS_OPTIONS_H
