#include <slas/dbus/object.h>

#include <sstream>
#include <boost/log/trivial.hpp>

#include <slas/dbus/bus.h>
#include <slas/dbus/detail/dbus_error_guard.h>
#include <slas/dbus/exception/dbus_loop_exception.h>


namespace dbus
{

Bus::Options::Options(const std::string &address,
                      const int &port,
                      const std::string &family)
: address_(address),
port_(port),
family_(family) {
}

const std::string Bus::Options::GetAddress() const {
  std::stringstream stream;
  std::string sport;

  stream << port_;
  stream >> sport;

  return "tcp:host=" + address_ + ",port=" + sport + ",family=" + family_;
}

Bus::Bus(const Bus::Options &options)
: options_(options),
loop_running_(false) {
  dbus_wrapper_ = detail::DBusWrapper::Create();
}

void Bus::Connect() {
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Connect: Function call";

  std::string address = options_.GetAddress();
  BOOST_LOG_TRIVIAL(info) << "Connecting to: " << address;

  dbus_wrapper_->ConnectionOpenPrivate(address);
  dbus_wrapper_->BusRegister();
  dbus_wrapper_->ConnectionSetExitOnDisconnect(false);
}

void Bus::Disconnect() {
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Disconnect: Function call";

  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Disconnect: Unregistering all objects";
  UnregisterAllObjects();

  dbus_wrapper_->ConnectionClose();
}

void Bus::RequestConnectionName(const std::string &method_name) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::RequestConnectionName: Function call";

  dbus_wrapper_->BusRequestName(method_name, DBUS_NAME_FLAG_REPLACE_EXISTING);
}

void Bus::RegisterObject(ObjectPtr object) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::RegisterObject: Function call";

  void *vobject = static_cast<void*> (object.get());

  const DBusObjectPathVTable dbus_vtable = {
    NULL, StaticMessageHandler, NULL, NULL, NULL, NULL
  };

  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::RegisterObject: Registering object";
  dbus_wrapper_->ConnectionRegisterObjectPath(object->GetPath(), &dbus_vtable, vobject);

  registered_objects_.push_back(object);
}

void Bus::UnregisterObject(ObjectPtr object) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::UnregisterObject: Function call";

  DBusUnregisterObject(object);

  auto it = find(registered_objects_.begin(),
                 registered_objects_.end(),
                 object);
  registered_objects_.erase(it);
}

void Bus::UnregisterAllObjects() {
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::UnregisterAllObject: Function call";

  for (auto o : registered_objects_)
    DBusUnregisterObject(o);

  registered_objects_.clear();
}

void Bus::Loop() {
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Loop: Function call";
  DBusMessage* msg;
  DBusDispatchStatus dispatch_status;

  loop_running_ = true;
  while (loop_running_) {
    dbus_wrapper_->ConnectionReadWrite(1000);

    msg = dbus_wrapper_->ConnectionBorrowMessage();

    if (msg == nullptr) {
      BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Loop: Data not received";
    }
    else {
      BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Loop: Received new data";

      BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Loop: Returning message to D-Bus";
      dbus_wrapper_->ConnectionReturnMessage(msg);

      BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Loop: Processing message";
      dispatch_status = dbus_wrapper_->ConnectionDispatch();
      if (dispatch_status == DBUS_DISPATCH_NEED_MEMORY) {
        BOOST_LOG_TRIVIAL(error) << "dbus::Bus::Loop: More memory is needed to continue";
        throw exception::DBusLoopException();
      }
      BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::Loop: Processed";
    }
  }
}

void Bus::StopLoop() {
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::StopLoop: Function call";
  loop_running_ = false;
}

void Bus::SendMessage(DBusMessage *message, DBusPendingCall **reply_handle) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::SendMessage: Function call";

  dbus_wrapper_->ConnectionSendWithReply(message, reply_handle, -1);

  BOOST_LOG_TRIVIAL(error) << "dbus::Bus::SendMessage: flush connection";
  dbus_wrapper_->ConnectionFlush();
}

DBusHandlerResult Bus::StaticMessageHandler(DBusConnection *connection, DBusMessage *message, void *user_data) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::StaticMessageHandler: Function call";
  Object *object = static_cast<Object*> (user_data);
  return object->MessageHandler(connection, message);
}

void Bus::DBusUnregisterObject(ObjectPtr object) {
  BOOST_LOG_TRIVIAL(debug) << "dbus::Bus::DBusUnregisterObject: Function call";

  dbus_wrapper_->ConnectionUnregisterObjectPath(object->GetPath());
}

}
