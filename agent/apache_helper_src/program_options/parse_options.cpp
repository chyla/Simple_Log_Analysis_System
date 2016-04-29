#include "parse_options.h"

#include <string>
#include <boost/program_options.hpp>

using namespace boost::program_options;
using namespace std;

namespace program_options
{

Options ParseOptions(int argc, char **argv) {
  options_description description;
  description.add_options()
      ("socket", value<string>(), "Apache module Unix socket path.")
      ("logfile", value<string>(), "Path to logfile.");

  variables_map variables;
  store(parse_command_line(argc,
                           argv,
                           description),
        variables);
  notify(variables);

  Options options;
  options.SetSocketPath(variables["socket"].as<string>());
  options.SetLogFilePath(variables["logfile"].as<string>());

  return options;
}

}
