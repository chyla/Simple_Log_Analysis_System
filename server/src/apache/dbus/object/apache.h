#pragma once

#include <vector>

#include <patlms/dbus/object.h>
#include <patlms/type/apache_log_entry.h>

#include "src/database/database.h"
#include "src/database/detail/general_database_functions_interface.h"
#include "src/apache/database/detail/database_functions_interface.h"

namespace apache
{

namespace dbus
{

namespace object
{

class Apache : public ::dbus::Object {
 public:
  Apache(::database::DatabasePtr database,
         ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
         ::apache::database::detail::DatabaseFunctionsInterfacePtr apache_database_functions);
  virtual ~Apache();

  const char* GetPath();

 protected:
  const char* GetXmlInterface();

  DBusHandlerResult OwnMessageHandler(DBusConnection *connection, DBusMessage *message);

  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;
  ::apache::database::detail::DatabaseFunctionsInterfacePtr apache_database_functions_;
  ::database::DatabasePtr database_;
};

typedef std::shared_ptr<Apache> ApachePtr;

}

}

}
