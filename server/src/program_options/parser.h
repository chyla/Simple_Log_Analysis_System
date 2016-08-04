#pragma once

#include <boost/program_options.hpp>
#include <utility>
#include <string>

#include "src/program_options/type/options.h"


namespace program_options
{

class Parser {
 public:
  Parser();

  std::string GetHelpMessage() const;

  void SetCommandLineOptions(const int &argc, char **argv);
  void SetConfigFilePath(const std::string &path);

  type::Options Parse();

 private:
  typedef boost::program_options::options_description options_description;

  std::pair<int, char**> cmd_params_;
  std::string config_file_path_;

  options_description general_options_, help_options_, all_options_;

  type::SecurityOption ToSecurityOption(const std::string &s);
};

}
