#ifndef PATLMS_PROGRAM_OPTIONS_OPTIONS_H
#define PATLMS_PROGRAM_OPTIONS_OPTIONS_H

#include <string>


namespace program_options
{

class Options {
 public:
  Options();

  static const Options Create(const std::string &run_as_user,
                              const std::string &pidfile_path,
                              const std::string &logfile_path,
                              const std::string &dbus_address,
                              unsigned dbus_port,
                              const std::string &dbus_family,
                              bool help_message,
                              bool daemon,
                              const std::string &databasefile_path,
                              const std::string &web_address,
                              unsigned web_port);

  const std::string& GetRunAsUser() const;
  const std::string& GetPidfilePath() const;
  const std::string& GetLogfilePath() const;

  const std::string& GetDbusAddress() const;
  const unsigned& GetDbusPort() const;
  const std::string& GetDbusFamily() const;

  const bool IsShowHelpMessage() const;
  bool IsDaemon() const;

  const std::string& GetDatabasefilePath() const;

  const std::string& GetWebAddress() const;
  const unsigned& GetWebPort() const;

 private:
  std::string run_as_user_;
  std::string pidfile_path_;
  std::string logfile_path_;
  std::string databasefile_path_;

  std::string web_address_;
  unsigned web_port_;

  std::string dbus_address_;
  unsigned dbus_port_;
  std::string dbus_family_;

  bool show_help_message_;
  bool daemon_;
};

}

#endif // PATLMS_PROGRAM_OPTIONS_OPTIONS_H
