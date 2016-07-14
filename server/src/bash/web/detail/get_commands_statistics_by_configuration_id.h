/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef GET_COMMANDS_STATISTICS_BY_CONFIGURATION_ID_H
#define GET_COMMANDS_STATISTICS_BY_CONFIGURATION_ID_H

#include "web_command.h"

#include "src/bash/domain/detail/web_scripts_interface.h"

#include <memory>

namespace bash
{

namespace web
{

namespace detail
{

class GetCommandsStatisticsByConfigurationId;
typedef std::shared_ptr<GetCommandsStatisticsByConfigurationId> GetCommandsStatisticsByConfigurationIdPtr;

class GetCommandsStatisticsByConfigurationId : public WebCommand {
 public:
  virtual ~GetCommandsStatisticsByConfigurationId() = default;

  static GetCommandsStatisticsByConfigurationIdPtr Create(::bash::domain::detail::WebScriptsInterfacePtr scripts);

  ::web::type::Command GetCommandName() override;

  ::web::type::JsonMessage Execute(const ::web::type::JsonMessage &message) override;

 private:
  ::bash::domain::detail::WebScriptsInterfacePtr scripts_;

  GetCommandsStatisticsByConfigurationId(::bash::domain::detail::WebScriptsInterfacePtr scripts);
};

}

}

}

#endif /* GET_COMMANDS_STATISTICS_BY_CONFIGURATION_ID_H */
