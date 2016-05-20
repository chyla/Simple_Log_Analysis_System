#ifndef LIBPATLMS_DBUS_BUS_H
#define LIBPATLMS_DBUS_BUS_H

#include <string>
#include <list>
#include <dbus/dbus.h>
#include <memory>

#include <patlms/dbus/object.h>
#include <patlms/dbus/detail/bus_interface.h>


namespace dbus
{
  
class Bus : public detail::BusInterface
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

  void RegisterObject(ObjectPtr object);
  void UnregisterObject(ObjectPtr object);
  void UnregisterAllObjects();

  void Loop();
  void StopLoop();

  bool SendMessage(DBusMessage *message, DBusPendingCall **reply_handle);

 private:
  const Options options_;
  DBusConnection *connection_;
  bool loop_running_;

  static DBusHandlerResult StaticMessageHandler(DBusConnection *connection, DBusMessage *message, void *user_data);

  std::list<ObjectPtr> registered_objects_;

  void DBusUnregisterObject(ObjectPtr object);
};

typedef std::shared_ptr<Bus> BusPtr;

}

#endif // LIBPATLMS_DBUS_BUS_H
