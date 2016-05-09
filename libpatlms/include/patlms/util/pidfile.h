#ifndef LIBPATLMS_UTIL_PIDFILE_H
#define LIBPATLMS_UTIL_PIDFILE_H

#include <string>
#include <memory>

#include <patlms/util/detail/system.h>

namespace util
{

void CreatePidFile(const std::string &pidfile_path, detail::SystemInterfacePtr system = std::make_shared<detail::System>());

void RemovePidFile(const std::string &pidfile_path, detail::SystemInterfacePtr system = std::make_shared<detail::System>());

}

#endif /* LIBPATLMS_UTIL_PIDFILE_H */
