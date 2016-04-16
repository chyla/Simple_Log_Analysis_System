#ifndef LIBPATLMS_DBUS_PROXY_OBJECT_H
#define LIBPATLMS_DBUS_PROXY_OBJECT_H

#include "bus.h"


namespace dbus
{

class ProxyObject
{
 public:
  ProxyObject();
  virtual ~ProxyObject();

 protected:
  virtual DBusMessage* CreateMethodCall(const char *bus_name,
                                        const char *object_name,
                                        const char *interface_name,
                                        const char *method_name);

  virtual void InitArgument(DBusMessage *message, DBusMessageIter *args);

  virtual bool AppendArgument(DBusMessageIter *iter_args,
                              const char *param);

  virtual DBusMessage* GetReplyMessage(DBusPendingCall *reply_handle);

  virtual void FreePendingCall(DBusPendingCall *reply_handle);
  
};
  
}

#endif // LIBPATLMS_DBUS_PROXY_OBJECT_H
