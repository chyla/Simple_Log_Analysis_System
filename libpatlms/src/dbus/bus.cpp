#include <patlms/dbus/object.h>

#include <sstream>
#include <boost/log/trivial.hpp>

#include <patlms/dbus/bus.h>
#include <patlms/dbus/detail/dbus_error_guard.h>


namespace dbus
{

Bus::Options::Options(const std::string &address,
		      const int &port,
		      const std::string &family)
  : address_(address),
    port_(port),
    family_(family)
{
}

const std::string Bus::Options::GetAddress() const
{
  std::stringstream stream;
  std::string sport;

  stream << port_;
  stream >> sport;
  
  return "tcp:host=" + address_ + ",port=" + sport + ",family=" + family_;
}

Bus::Bus(const Bus::Options &options)
  : options_(options)
{
}

bool Bus::Connect()
{
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Connect: Function call";

  if (!connection_) {
    BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Connect: Connection already initialized";
    return false;
  }

  DBusError error;
  DBusErrorGuard error_guard(&error);

  std::string address = options_.GetAddress();
  BOOST_LOG_TRIVIAL(info) << "Connecting to: " << address;
  connection_ = dbus_connection_open_private(address.c_str(), &error);

  if (!connection_) {
    BOOST_LOG_TRIVIAL(error) << "dbus::Bus::Connect: Failed to connect to the bus: " << (error.message != nullptr ? error.message : "");
    return false;
  }

  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Connect: dbus_bus_register function call";
  bool reg = dbus_bus_register(connection_, &error);
  if (!reg) {
      BOOST_LOG_TRIVIAL(error) << "dbus::Bus::Connect: Failed to call dbus_bus_register: " << (error.message != nullptr ? error.message : "");
    return false;
  }

  dbus_connection_set_exit_on_disconnect(connection_, false);

  return true;
}

bool Bus::Disconnect()
{
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Disconnect: Function call";

  if (!connection_)
  {
    BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Disconnect: Not connected";
    return false;
  }
  
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Disconnect: Unregistering all objects";
  UnregisterAllObjects();

  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Disconnect: dbus_connection_close function call";
  dbus_connection_close(connection_);
  connection_ = nullptr;

  return true;
}


bool Bus::RequestConnectionName(const std::string &method_name)
{
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::RequestConnectionName: Function call";

  if (!connection_) {
    BOOST_LOG_TRIVIAL(error) << "dbus::Bus::RequestConnectionName: Not connected to the bus";
    return false;
  }

  DBusError error;
  DBusErrorGuard error_guard(&error);

  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::RequestConnectionName: dbus_bus_request_name function call";
  dbus_bus_request_name(connection_, method_name.c_str(), DBUS_NAME_FLAG_REPLACE_EXISTING, &error);

  if (dbus_error_is_set(&error)) {
    BOOST_LOG_TRIVIAL(error) << "dbus::Bus::RequestConnectionName: " <<  (error.message != nullptr ? error.message : "");
    return false;
  }

  return true;
}

void Bus::RegisterObject(Object *object)
{
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::RegisterObject: Function call";

  void *vobject = static_cast<void*>(object);
  
  const DBusObjectPathVTable dbus_vtable = {
    NULL, StaticMessageHandler, NULL, NULL, NULL, NULL
  };

  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::RegisterObject: Registering object";
  dbus_connection_register_object_path(connection_, object->GetPath(),
				       &dbus_vtable, vobject);

  registered_objects_.push_back(object);
}

void Bus::UnregisterObject(Object *object)
{
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::UnregisterObject: Function call";

  DBusUnregisterObject(object);

  auto it = find(registered_objects_.begin(),
		 registered_objects_.end(),
		 object);
  registered_objects_.erase(it);
}

void Bus::UnregisterAllObjects()
{
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::UnregisterAllObject: Function call";

  for (Object *o : registered_objects_)
    DBusUnregisterObject(o);

  registered_objects_.clear();
}

void Bus::Loop()
{
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Loop: Function call";
  
  while (dbus_connection_read_write_dispatch(connection_, -1))
    ;
}

bool Bus::SendMessage(DBusMessage *message, DBusPendingCall **reply_handle)
{
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::SendMessage: Function call";

  bool ret = dbus_connection_send_with_reply(connection_, message, reply_handle, -1);

  if (!ret)
  {
    BOOST_LOG_TRIVIAL(error) << "dbus::Bus::SendMessage: Failed to send message: out of memory";
    return false;
  }

  if (*reply_handle == nullptr)
  {
    BOOST_LOG_TRIVIAL(error) << "dbus::Bus::SendMessage: reply_handle is null";
    return false;
  }

  BOOST_LOG_TRIVIAL(error) << "dbus::Bus::SendMessage: flush connection";
  dbus_connection_flush(connection_);
  
  return true;
}

DBusHandlerResult Bus::StaticMessageHandler(DBusConnection *connection, DBusMessage *message, void *user_data)
{
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::StaticMessageHandler: Function call";
  Object *object = static_cast<Object*>(user_data);
  return object->MessageHandler(connection, message);
}

void Bus::DBusUnregisterObject(Object *object)
{
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::DBusUnregisterObject: Function call";

  bool ret = dbus_connection_unregister_object_path(connection_,
						    object->GetPath());
  if (!ret)
    BOOST_LOG_TRIVIAL(error) << "dbus::Bus::DBusUnregisterObject: Not enough memory";
}

}
