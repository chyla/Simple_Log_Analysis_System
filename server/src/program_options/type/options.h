#ifndef SRC_PROGRAM_OPTIONS_TYPE_OPTIONS_H
#define SRC_PROGRAM_OPTIONS_TYPE_OPTIONS_H

#include <string>

#include "security_option.h"

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
                              SecurityOption mail_server_secure,
                              std::string mail_server_address,
                              unsigned mail_server_port,
                              std::string mail_server_user,
                              std::string mail_server_password,
                              std::string mail_to,
                              std::string mail_from,
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

  SecurityOption GetMailServerSecure() const;
  const std::string& GetMailServerAddress() const;
  unsigned GetMailServerPort() const;
  const std::string& GetMailServerUser() const;
  const std::string& GetMailServerPassword() const;
  const std::string& GetMailTo() const;
  const std::string& GetMailFrom() const;

 private:
  std::string run_as_user_;
  std::string pidfile_path_;
  std::string logfile_path_;
  std::string databasefile_path_;

  std::string web_address_;
  unsigned web_port_;

  SecurityOption mail_server_secure_;
  std::string mail_server_address_;
  unsigned mail_server_port_;
  std::string mail_server_user_;
  std::string mail_server_password_;
  std::string mail_to_;
  std::string mail_from_;

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
