/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include "system_interface.h"

namespace apache
{

namespace analyzer
{

namespace detail
{

class System;
typedef std::shared_ptr<System> SystemPtr;

class System : public SystemInterface {
 public:
  virtual ~System() = default;
  
  static SystemPtr Create();

  time_t Time(time_t *t) override;

  struct tm* LocalTime(const time_t *timep) override;
};

}

}

}

#endif /* SYSTEM_H */
