#include "options.h"

#include <regex>

#include <slas/util/path.h>
#include <slas/type/exception/detail/wrong_path_exception.h>

using type::exception::detail::WrongPathException;

namespace program_options
{

void Options::SetSocketPath(const std::string &socket_path) {
  if (!util::IsAbsoluteUnixPath(socket_path))
    throw WrongPathException();
  
  socket_path_ = socket_path;
}

void Options::SetLogFilePath(const std::string &log_file_path) {
  if (!util::IsAbsoluteUnixPath(log_file_path))
    throw WrongPathException();
  
  log_file_path_ = log_file_path;
}

}
