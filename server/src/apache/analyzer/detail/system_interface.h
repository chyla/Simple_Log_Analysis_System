/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SYSTEM_INTERFACE_H
#define SYSTEM_INTERFACE_H

#include <memory>
#include <ctime>

namespace apache
{

namespace analyzer
{

namespace detail
{

class SystemInterface {
 public:
  virtual ~SystemInterface() = default;

  virtual time_t Time(time_t *t) = 0;

  virtual struct tm* LocalTime(const time_t *timep) = 0;
};

typedef std::shared_ptr<SystemInterface> SystemInterfacePtr;

}

}

}

#endif /* SYSTEM_INTERFACE_H */
