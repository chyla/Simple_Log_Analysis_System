#ifndef LIBPATLMS_UTIL_SYSTEM_H
#define LIBPATLMS_UTIL_SYSTEM_H

#include <patlms/util/detail/system_interface.h>

namespace util
{

namespace detail
{

class System : public SystemInterface {
 public:
  virtual ~System() = default;

  FILE* fopen(const char *path, const char *mode) override;

  size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) override;

  int fclose(FILE *fp) override;

  pid_t getpid(void) override;

  int unlink(const char *pathname) override;

  int access(const char *pathname, int mode) override;
};

}

}

#endif /* LIBPATLMS_UTIL_SYSTEM_H */
