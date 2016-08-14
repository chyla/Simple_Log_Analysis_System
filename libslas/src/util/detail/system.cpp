#include <slas/util/detail/system.h>

#include <boost/log/trivial.hpp>

namespace util
{

namespace detail
{

FILE* System::fopen(const char *path, const char *mode) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::util::detail::System::fopen: Function call with (path=" << path << "; mode=" << mode << ")";
  return ::fopen(path, mode);
}

size_t System::fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::util::detail::System::fwrite: Function call with (ptr*; size=" << size << "; nmemb=" << nmemb << "; stream=*)";
  return ::fwrite(ptr, size, nmemb, stream);
}

int System::fclose(FILE *fp) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::util::detail::System::fclose: Function call with (fp=*)";
  return ::fclose(fp);
}

pid_t System::getpid(void) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::util::detail::System::getpid: Function call";
  return ::getpid();
}

int System::unlink(const char *pathname) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::util::detail::System::getpid: Function call with (pathname=" << pathname << ")";
  return ::unlink(pathname);
}

int System::access(const char *pathname, int mode) {
  BOOST_LOG_TRIVIAL(debug) << "libpatlms::util::detail::System::stat: Function call with (pathname=" << pathname << "; buf=*)";
  return ::access(pathname, mode);
}

}

}
