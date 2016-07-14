/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SELECT_COMMANDS_H
#define SELECT_COMMANDS_H

#include "web_command.h"

#include "src/bash/domain/detail/web_scripts_interface.h"

#include <memory>

namespace bash
{

namespace web
{

namespace detail
{

class SaveSelectedCommands;
typedef std::shared_ptr<SaveSelectedCommands> SaveSelectedCommandsPtr;

class SaveSelectedCommands : public WebCommand {
 public:
  virtual ~SaveSelectedCommands() = default;

  static SaveSelectedCommandsPtr Create(::bash::domain::detail::WebScriptsInterfacePtr scripts);

  ::web::type::Command GetCommandName() override;

  ::web::type::JsonMessage Execute(const ::web::type::JsonMessage &message) override;

 private:
  ::bash::domain::detail::WebScriptsInterfacePtr scripts_;

  SaveSelectedCommands(::bash::domain::detail::WebScriptsInterfacePtr scripts);
};

}

}

}

#endif /* SELECT_COMMANDS_H */
