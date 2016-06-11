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
  
  virtual void Connect() = 0;
  virtual void Disconnect() = 0;

  virtual void RequestConnectionName(const std::string &method_name) = 0;

  virtual void RegisterObject(ObjectPtr object) = 0;
  virtual void UnregisterObject(ObjectPtr object) = 0;
  virtual void UnregisterAllObjects() = 0;

  virtual void Loop() = 0;

  virtual void SendMessage(DBusMessage *message, DBusPendingCall **reply_handle) = 0;
};

typedef std::shared_ptr<BusInterface> BusInterfacePtr;

}

}

#endif /* LIBPATLMS_DBUS_BUS_INTERFACE_H */
