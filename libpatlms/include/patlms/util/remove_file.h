#ifndef LIBPATLMS_UTIL_REMOVE_FILE_H
#define LIBPATLMS_UTIL_REMOVE_FILE_H

#include <patlms/util/detail/system.h>

namespace util
{

void RemoveFile(const std::string &path, detail::SystemInterfacePtr system = std::make_shared<detail::System>());

}

#endif /* LIBPATLMS_UTIL_REMOVE_FILE_H */
