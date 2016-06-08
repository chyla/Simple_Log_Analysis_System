#ifndef PATLMS_DBUS_DETAIL_SYSTEM_H
#define PATLMS_DBUS_DETAIL_SYSTEM_H

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

#endif /* PATLMS_DBUS_DETAIL_SYSTEM_H */
