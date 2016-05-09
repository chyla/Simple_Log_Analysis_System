#ifndef LIBPATLMS_DBUS_BUS_INTERFACE_H
#define LIBPATLMS_DBUS_BUS_INTERFACE_H

#include <dbus/dbus.h>
#include <string>
#include <memory>

#include <patlms/dbus/object.h>

namespace dbus
{

namespace detail
{

class BusInterface {
 public:
  virtual ~BusInterface();
  
  virtual bool Connect() = 0;
  virtual bool Disconnect() = 0;

  virtual bool RequestConnectionName(const std::string &method_name) = 0;

  virtual void RegisterObject(Object *object) = 0;
  virtual void UnregisterObject(Object *object) = 0;
  virtual void UnregisterAllObjects() = 0;

  virtual void Loop() = 0;

  virtual bool SendMessage(DBusMessage *message, DBusPendingCall **reply_handle) = 0;
};

typedef std::shared_ptr<BusInterface> BusInterfacePtr;

}

}

#endif /* LIBPATLMS_DBUS_BUS_INTERFACE_H */
