#ifndef PATLMS_MOCK_DETAIL_SYSTEM_H
#define PATLMS_MOCK_DETAIL_SYSTEM_H

#include "src/dbus/detail/system_interface.h"

#include <gmock/gmock.h>

namespace mock
{

namespace dbus
{

namespace detail
{

class System : public ::dbus::detail::SystemInterface
{
 public:
  MOCK_METHOD1(Usleep, void(unsigned));
  MOCK_METHOD1(Sleep, void(unsigned));
};

}

}

}

#endif /* PATLMS_MOCK_DETAIL_SYSTEM_H */
