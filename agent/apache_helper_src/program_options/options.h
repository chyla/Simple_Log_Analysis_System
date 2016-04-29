#ifndef APACHE_HELPER_PROGRAM_OPTIONS_OPTIONS_H
#define APACHE_HELPER_PROGRAM_OPTIONS_OPTIONS_H

#include <string>

namespace program_options
{

class Options {
 public:
  void SetSocketPath(const std::string &socket_path);
  void SetLogFilePath(const std::string &log_file_path);

  inline const std::string& GetSocketPath() const;
  inline const std::string& GetLogFilePath() const;

 private:
  std::string socket_path_;
  std::string log_file_path_;
};

const std::string& Options::GetSocketPath() const {
  return socket_path_;
}

const std::string& Options::GetLogFilePath() const {
  return log_file_path_;
}

}

#endif /* OPTIONS_H */
