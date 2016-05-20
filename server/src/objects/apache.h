#ifndef APACHE_H
#define APACHE_H

#include <vector>
#include <patlms/dbus/object.h>
#include <patlms/type/apache_log_entry.h>

#include "src/database/database.h"
#include "src/type/typedefs.h"

namespace objects
{

class Apache : public dbus::Object {
 public:
  Apache(database::DatabasePtr database);
  virtual ~Apache();

  const char* GetPath();

  void FlushCache();

 protected:
  const char* GetXmlInterface();

  DBusHandlerResult OwnMessageHandler(DBusConnection *connection, DBusMessage *message);

  database::DatabasePtr database_;
  type::ApacheLogs log_entry_cache_;
};

typedef std::shared_ptr<Apache> ApachePtr;

}

#endif /* APACHE_H */
