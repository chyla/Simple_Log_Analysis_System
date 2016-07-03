/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SCRIPTS_INTERFACE_H
#define SCRIPTS_INTERFACE_H

#include <patlms/type/bash_log_entry.h>

#include <memory>

namespace bash
{

namespace domain
{

namespace detail
{

class ScriptsInterface {
 public:
  virtual ~ScriptsInterface() = default;

  virtual void AddLog(const ::type::BashLogEntry &log_entry) = 0;
  
  virtual void CreateDailySystemStatistics() = 0;
};

typedef std::shared_ptr<ScriptsInterface> ScriptsInterfacePtr;

}

}

}

#endif /* SCRIPTS_H */
