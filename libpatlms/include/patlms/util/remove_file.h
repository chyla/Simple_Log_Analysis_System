#pragma once

#include <patlms/util/detail/system.h>

namespace util
{

void RemoveFile(const std::string &path, detail::SystemInterfacePtr system = std::make_shared<detail::System>());

}
