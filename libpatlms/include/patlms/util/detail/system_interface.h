#pragma once

#include <memory>
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>

namespace util
{

namespace detail
{

class SystemInterface {
 public:
  virtual ~SystemInterface() = default;

  virtual FILE* fopen(const char *path, const char *mode) = 0;

  virtual size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) = 0;

  virtual int fclose(FILE *fp) = 0;

  virtual pid_t getpid(void) = 0;

  virtual int unlink(const char *pathname) = 0;

  virtual int access(const char *pathname, int mode) = 0;
};

typedef std::shared_ptr<SystemInterface> SystemInterfacePtr;

}

}
