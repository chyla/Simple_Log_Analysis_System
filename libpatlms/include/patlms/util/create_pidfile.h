#pragma once

#include <patlms/util/detail/system.h>

#include <string>

namespace util
{

void CreatePidFile(const std::string &pidfile_path, detail::SystemInterfacePtr system = std::make_shared<detail::System>());

}
