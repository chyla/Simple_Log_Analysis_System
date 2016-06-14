#ifndef SRC_PROGRAM_OPTIONS_TYPE_OPTIONS_H
#define SRC_PROGRAM_OPTIONS_TYPE_OPTIONS_H

#include <string>

namespace program_options
{

namespace type
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
                              unsigned web_port,
                              bool debug);

  const std::string& GetRunAsUser() const;
  const std::string& GetPidfilePath() const;
  const std::string& GetLogfilePath() const;

  const std::string& GetDbusAddress() const;
  const unsigned& GetDbusPort() const;
  const std::string& GetDbusFamily() const;

  bool IsShowHelpMessage() const;
  bool IsDaemon() const;
  bool IsDebug() const;

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
  bool debug_;
};

}

}

#endif // SRC_PROGRAM_OPTIONS_TYPE_OPTIONS_H
