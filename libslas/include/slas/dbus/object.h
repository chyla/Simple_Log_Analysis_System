#pragma once

#include <memory>
#include <dbus/dbus.h>

namespace dbus
{

class Object {
 public:
  virtual ~Object();

  virtual const char* GetPath() = 0;

  DBusHandlerResult MessageHandler(DBusConnection *connection, DBusMessage *message);

 protected:
  virtual const char* GetXmlInterface() = 0;

  virtual DBusHandlerResult OwnMessageHandler(DBusConnection *connection, DBusMessage *message) = 0;

};

typedef std::shared_ptr<Object> ObjectPtr;

}
