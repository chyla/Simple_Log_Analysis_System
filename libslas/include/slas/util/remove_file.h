#pragma once

#include <slas/util/detail/system.h>

namespace util
{

void RemoveFile(const std::string &path, detail::SystemInterfacePtr system = std::make_shared<detail::System>());

}
