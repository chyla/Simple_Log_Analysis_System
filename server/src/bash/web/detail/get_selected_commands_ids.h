/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef GET_MARKED_COMMANDS_IDS_H
#define GET_MARKED_COMMANDS_IDS_H

#include "web_command.h"

#include "src/bash/domain/detail/web_scripts_interface.h"

#include <memory>

namespace bash
{

namespace web
{

namespace detail
{

class GetSelectedCommandsIds;
typedef std::shared_ptr<GetSelectedCommandsIds> GetSelectedCommandsIdsPtr;

class GetSelectedCommandsIds : public WebCommand {
 public:
  virtual ~GetSelectedCommandsIds() = default;

  static GetSelectedCommandsIdsPtr Create(::bash::domain::detail::WebScriptsInterfacePtr scripts);

  ::web::type::Command GetCommandName() override;

  ::web::type::JsonMessage Execute(const ::web::type::JsonMessage &message) override;

 private:
  ::bash::domain::detail::WebScriptsInterfacePtr scripts_;

  GetSelectedCommandsIds(::bash::domain::detail::WebScriptsInterfacePtr scripts);
};

}

}

}

#endif /* GET_MARKED_COMMANDS_IDS_H */
