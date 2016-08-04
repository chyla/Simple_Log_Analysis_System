/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include <memory>
#include <ctime>

namespace notifier
{

namespace detail
{

class SystemInterface {
 public:
  virtual ~SystemInterface() = default;

  virtual void Sleep(unsigned seconds) = 0;

  virtual time_t Time() = 0;

  virtual tm* GMTime(const time_t *timep) = 0;
};

typedef std::shared_ptr<SystemInterface> SystemInterfacePtr;

}

}
