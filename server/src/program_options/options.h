#ifndef PATLMS_PROGRAM_OPTIONS_OPTIONS_H
#define PATLMS_PROGRAM_OPTIONS_OPTIONS_H

#include <string>


namespace program_options
{

class Options
{
 public:
  Options();

  const std::string& GetRunAsUser() const;
  const std::string& GetPidfilePath() const;

  const std::string& GetDbusAddress() const;
  const unsigned& GetDbusPort() const;
  const std::string& GetDbusFamily() const;

  const bool GetShowHelpMessage() const;

  void SetRunAsUser(const std::string &username);
  void SetPidfilePath(const std::string &path);

  void SetDbusAddress(const std::string &address);
  void SetDbusPort(const unsigned &port);
  void SetDbusFamily(const std::string &family);

  void SetShowHelpMessage(bool show);
  
 private:
  std::string run_as_user_;
  std::string pidfile_path_;

  std::string dbus_address_;
  unsigned dbus_port_;
  std::string dbus_family_;

  bool show_help_message_;
};

}

#endif // PATLMS_PROGRAM_OPTIONS_OPTIONS_H
