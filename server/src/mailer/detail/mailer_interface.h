/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "mail_interface.h"

#include <memory>

namespace mailer
{

namespace detail
{

class MailerInterface {
 public:
  virtual ~MailerInterface() = default;

  virtual void Send(const MailInterfacePtr mail) = 0;
};

typedef std::shared_ptr<MailerInterface> MailerInterfacePtr;

}

}
