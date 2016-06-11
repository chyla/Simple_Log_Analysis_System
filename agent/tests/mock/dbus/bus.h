#ifndef PATLMS_MOCK_BUS_BUS_H
#define PATLMS_MOCK_BUS_BUS_H

#include <gmock/gmock.h>

#include <patlms/dbus/detail/bus_interface.h>
#include <patlms/dbus/object.h>

namespace mock
{

namespace dbus
{

class Bus : public ::dbus::detail::BusInterface {
  MOCK_METHOD0(Connect, void());
  MOCK_METHOD0(Disconnect, void());

  MOCK_METHOD1(RequestConnectionName, void(const std::string &method_name));

  MOCK_METHOD1(RegisterObject, void(::dbus::ObjectPtr object));
  MOCK_METHOD1(UnregisterObject, void(::dbus::ObjectPtr object));
  MOCK_METHOD0(UnregisterAllObjects, void());

  MOCK_METHOD0(Loop, void());

  MOCK_METHOD2(SendMessage, void(DBusMessage *message, DBusPendingCall **reply_handle));
};

}

}

#endif /* PATLMS_MOCK_BUS_BUS_H */
