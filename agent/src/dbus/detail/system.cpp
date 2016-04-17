#include "system.h"

#include <unistd.h>

namespace dbus
{

namespace detail
{

void System::Sleep(unsigned seconds) {
  sleep(seconds);
}

}

}