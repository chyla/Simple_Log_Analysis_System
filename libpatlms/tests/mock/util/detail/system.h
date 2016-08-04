#pragma once

#include <gmock/gmock.h>

#include <patlms/util/detail/system_interface.h>

namespace mock
{

namespace util
{

namespace detail
{

class System : public ::util::detail::SystemInterface {
 public:
  MOCK_METHOD2(fopen, FILE*(const char *path, const char *mode));

  MOCK_METHOD4(fwrite, size_t(const void *ptr, size_t size, size_t nmemb, FILE *stream));

  MOCK_METHOD1(fclose, int(FILE *fp));

  MOCK_METHOD0(getpid, pid_t(void));

  MOCK_METHOD1(unlink, int(const char *pathname));

  MOCK_METHOD2(access, int(const char *pathname, int mode));
};

}

}

}
