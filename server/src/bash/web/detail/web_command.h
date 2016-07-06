/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef COMMAND_H
#define COMMAND_H

#include "src/web/type/command.h"
#include "src/web/type/json_message.h"

#include <memory>

namespace bash
{

namespace web
{

namespace detail
{

class WebCommand {
 public:
  virtual ~WebCommand() = default;

  virtual ::web::type::Command GetCommandName() = 0;

  virtual ::web::type::JsonMessage Execute(const ::web::type::JsonMessage &message) = 0;
};

typedef std::shared_ptr<WebCommand> WebCommandPtr;

}

}

}

#endif /* COMMAND_H */
