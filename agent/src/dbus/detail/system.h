#pragma once

#include "system_interface.h"

namespace dbus
{

namespace detail
{

class System : public SystemInterface {
 public:
  void Usleep(unsigned ms);
  void Sleep(unsigned seconds);
};

}

}
