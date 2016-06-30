/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef NOTIFIER_TYPE_NOTIFIER_MESSAGE_INTERFACE_H
#define NOTIFIER_TYPE_NOTIFIER_MESSAGE_INTERFACE_H

#include <string>
#include <memory>

namespace notifier
{

namespace type
{

class NotifierMessageInterface {
 public:
  virtual ~NotifierMessageInterface() = default;

  virtual std::string GetModuleName() = 0;
  virtual std::string GetDetectionResults() = 0;
};

typedef std::shared_ptr<NotifierMessageInterface> NotifierMessageInterfacePtr;

}

}

#endif /* NOTIFIER_TYPE_NOTIFIER_MESSAGE_INTERFACE_H */
