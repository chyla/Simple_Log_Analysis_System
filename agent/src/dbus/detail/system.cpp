#include "system.h"

#include <unistd.h>

namespace dbus
{

namespace detail
{

void System::Usleep(unsigned ms) {
  usleep(ms);
}


void System::Sleep(unsigned seconds) {
  sleep(seconds);
}

}

}
