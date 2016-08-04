/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "system_interface.h"

namespace notifier
{

namespace detail
{

class System;
typedef std::shared_ptr<System> SystemPtr;

class System : public SystemInterface {
 public:
  static SystemPtr Create();

  void Sleep(unsigned seconds) override;

  time_t Time() override;

  tm* GMTime(const time_t *timep) override;
};

}

}
