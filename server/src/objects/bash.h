#ifndef PATLMS_OBJECTS_BASH_H
#define PATLMS_OBJECTS_BASH_H

#include "dbus/object.h"


namespace objects
{

class Bash : public dbus::Object
{
 public:
  virtual ~Bash();

  const char* GetPath();

 protected:
  const char* GetXmlInterface();

  DBusHandlerResult OwnMessageHandler(DBusConnection *connection, DBusMessage *message);

};

}
  
#endif // PATLMS_OBJECTS_BASH_H
