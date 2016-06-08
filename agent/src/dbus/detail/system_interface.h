#ifndef PATLMS_DBUS_DETAIL_SYSTEM_INTERFACE_H
#define PATLMS_DBUS_DETAIL_SYSTEM_INTERFACE_H

#include <memory>

namespace dbus
{

namespace detail
{

class SystemInterface {
 public:
  virtual void Usleep(unsigned ms) = 0;
  virtual void Sleep(unsigned seconds) = 0;
};

typedef std::shared_ptr<SystemInterface> SystemInterfacePtr;

}

}

#endif /* PATLMS_DBUS_DETAIL_SYSTEM_INTERFACE_H */
