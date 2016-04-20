#ifndef PATLMS_MOCK_DBUS_DBUS_THREAD_H
#define PATLMS_MOCK_DBUS_DBUS_THREAD_H

#include <gmock/gmock.h>

#include "src/dbus/detail/dbus_thread_interface.h"

namespace mock
{

namespace dbus
{

class DBusThread : public ::dbus::detail::DBusThreadInterface {
 public:
  MOCK_METHOD1(AddCommand, void (::dbus::detail::DBusThreadInterface::CommandPtr command));

  MOCK_METHOD0(StartLoop, void());
  MOCK_METHOD0(StopLoop, void());

  MOCK_METHOD0(IsLoopRunning, bool());
};

}

}

#endif /* PATLMS_MOCK_DBUS_DBUS_THREAD_H */
