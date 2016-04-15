#ifndef LIBPATLMS_DBUS_BUS_H
#define LIBPATLMS_DBUS_BUS_H

#include <string>
#include <list>
#include <dbus/dbus.h>

#include "object.h"


namespace dbus
{
  
class Bus
{
 public:
  class Options
  {
  public:
    Options(const std::string &address,
	    const int &port,
	    const std::string &family);

    const std::string GetAddress() const;

  private:
    const std::string address_;
    const int port_;
    const std::string family_;
  };

  explicit Bus(const Options &options);

  bool Connect();
  bool Disconnect();

  bool RequestConnectionName(const std::string &method_name);

  void RegisterObject(Object *object);
  void UnregisterObject(Object *object);
  void UnregisterAllObjects();

  void Loop();

  bool SendMessage(DBusMessage *message, DBusPendingCall **reply_handle);

 private:
  const Options options_;
  DBusConnection *connection_;

  static DBusHandlerResult StaticMessageHandler(DBusConnection *connection, DBusMessage *message, void *user_data);

  std::list<Object*> registered_objects_;

  void DBusUnregisterObject(Object *object);
};

}

#endif // LIBPATLMS_DBUS_BUS_H
