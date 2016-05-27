#ifndef PATLMS_OBJECTS_BASH_H
#define PATLMS_OBJECTS_BASH_H

#include <vector>
#include <memory>
#include <patlms/dbus/object.h>
#include <patlms/type/bash_log_entry.h>

#include "src/database/database.h"

namespace objects
{

class Bash : public dbus::Object {
 public:
  Bash(database::DatabasePtr database);
  virtual ~Bash();

  const char* GetPath();

  void FlushCache();

 protected:
  const char* GetXmlInterface();

  DBusHandlerResult OwnMessageHandler(DBusConnection *connection, DBusMessage *message);

  database::DatabasePtr database_;
  type::BashLogs log_entry_cache_;
};

typedef std::shared_ptr<Bash> BashPtr;

}

#endif // PATLMS_OBJECTS_BASH_H
