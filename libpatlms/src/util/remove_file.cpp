#include <patlms/util/remove_file.h>
#include <patlms/util/detail/cant_remove_file_exception.h>

#include <string>
#include <cstring>
#include <cerrno>
#include <boost/log/trivial.hpp>

using namespace std;

namespace util
{

void RemoveFile(const std::string &path, detail::SystemInterfacePtr system) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::util::RemovePidFile: Function call with (path=" << path << ")";

  int ret = system->unlink(path.c_str());
  if (ret < 0) {
    BOOST_LOG_TRIVIAL(error) << "libpatlms::util::RemovePidFile: Couldn't remove pid file: " << strerror(errno);
    throw detail::CantRemoveFileException();
  }

  BOOST_LOG_TRIVIAL(debug) << "libpatlms::util::RemovePidFile: Done";
}

}
