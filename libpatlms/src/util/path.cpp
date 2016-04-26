#include <patlms/util/path.h>

#include <regex>

using namespace std;

namespace util
{

bool IsAbsoluteUnixPath(const std::string& path) {
  regex reg("/+([A-Za-z_/-]+/?)+");

  if (regex_match(path, reg))
    return true;

  return false;
}

}
