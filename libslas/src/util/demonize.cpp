#include <slas/util/demonize.h>
#include <slas/util/detail/demonize_exception.h>

#include <boost/log/trivial.hpp>

#include <cerrno>
#include <cstring>
#include <unistd.h>

namespace util
{

void Demonize(bool should_be_daemon) {
  if (should_be_daemon) {
    int ret = daemon(0, 0);
    if (ret < 0) {
      BOOST_LOG_TRIVIAL(info) << "libpatlms::util::Demonize: Can't daemonize: " << strerror(errno);
      throw detail::DemonizeException();
    }
  }
}

}
