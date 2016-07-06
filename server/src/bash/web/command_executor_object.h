/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef COMMAND_EXECUTOR_OBJECT_H
#define COMMAND_EXECUTOR_OBJECT_H

#include "src/web/type/command_executor_object_interface.h"

#include "src/bash/domain/detail/web_scripts_interface.h"
#include "src/bash/web/detail/web_command.h"

#include <map>
#include <memory>

namespace bash
{

namespace web
{

class CommandExecutorObject;
typedef std::shared_ptr<CommandExecutorObject> CommandExecutorObjectPtr;

class CommandExecutorObject : public ::web::type::CommandExecutorObjectInterface {
 public:
  virtual ~CommandExecutorObject() = default;

  static CommandExecutorObjectPtr Create(::bash::domain::detail::WebScriptsInterfacePtr scripts);

  const ::web::type::JsonMessage Execute(const ::web::type::JsonMessage &message);

  bool IsCommandSupported(const ::web::type::Command &command);

 private:
  ::bash::domain::detail::WebScriptsInterfacePtr scripts_;

  std::map<::web::type::Command, detail::WebCommandPtr> commands_;

  CommandExecutorObject(::bash::domain::detail::WebScriptsInterfacePtr scripts);
};

}

}

#endif /* COMMAND_EXECUTOR_OBJECT_H */
