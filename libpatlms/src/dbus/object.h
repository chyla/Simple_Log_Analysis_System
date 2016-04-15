#ifndef LIBPATLMS_DBUS_OBJECT_H
#define LIBPATLMS_DBUS_OBJECT_H

#include <dbus/dbus.h>


namespace dbus
{

class Object
{
 public:
  virtual ~Object();

  virtual const char* GetPath() = 0;

  DBusHandlerResult MessageHandler(DBusConnection *connection, DBusMessage *message);

 protected:
  virtual const char* GetXmlInterface() = 0;

  virtual DBusHandlerResult OwnMessageHandler(DBusConnection *connection, DBusMessage *message) = 0;

};

}

#endif // LIBPATLMS_DBUS_OBJECT_H
