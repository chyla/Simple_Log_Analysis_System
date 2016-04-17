#ifndef PATLMS_DBUS_DETAIL_SYSTEM_INTERFACE_H
#define PATLMS_DBUS_DETAIL_SYSTEM_INTERFACE_H

namespace dbus
{

namespace detail
{

class SystemInterface {
 public:
  virtual void Sleep(unsigned seconds) = 0;
};

}

}

#endif /* PATLMS_DBUS_DETAIL_SYSTEM_INTERFACE_H */
