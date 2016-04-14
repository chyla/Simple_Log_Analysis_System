#ifndef PATLMS_OBJECTS_BASH_PROXY_H
#define PATLMS_OBJECTS_BASH_PROXY_H

#include <string>

#include <patlms/dbus/proxy_object.h>


namespace objects
{
  
class BashProxy : public dbus::ProxyObject
{
 public:
  BashProxy(dbus::Bus &bus);

  bool AddLogEntry(const std::string &log_entry);

 private:
  dbus::Bus &bus_;
};

}

#endif // PATLMS_OBJECTS_BASH_PROXY_H
