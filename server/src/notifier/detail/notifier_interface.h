/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef NOTIFIER_DETAIL_NOTIFIER_INTERFACE_H
#define NOTIFIER_DETAIL_NOTIFIER_INTERFACE_H

#include <memory>

#include "src/notifier/type/notifier_messages.h"

namespace notifier
{

namespace detail
{

class NotifierInterface {
 public:
  virtual ~NotifierInterface() = default;

  virtual void Loop() = 0;
  virtual void StopLoop() = 0;
  virtual bool IsRunning() const = 0;

  virtual void AddMessages(::notifier::type::NotifierMessages messages) = 0;
};

}

}

#endif /* NOTIFIER_DETAIL_NOTIFIER_INTERFACE_H */
