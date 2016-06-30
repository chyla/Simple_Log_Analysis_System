/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "system.h"

#include <unistd.h>

namespace notifier
{

namespace detail
{

SystemPtr System::Create() {
  return std::make_shared<System>();
}

void System::Sleep(unsigned seconds) {
  sleep(seconds);
}

time_t System::Time() {
  return time(nullptr);
}

tm* System::GMTime(const time_t *timep) {
  return gmtime(timep);
}

}

}
