/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef GET_COMMANDS_STATISTICS_H
#define GET_COMMANDS_STATISTICS_H

#include "web_command.h"

#include "src/bash/domain/detail/web_scripts_interface.h"

#include <memory>

namespace bash
{

namespace web
{

namespace detail
{

class GetCommandsStatistics;
typedef std::shared_ptr<GetCommandsStatistics> GetCommandsStatisticsPtr;

class GetCommandsStatistics : public WebCommand {
 public:
  virtual ~GetCommandsStatistics() = default;

  static GetCommandsStatisticsPtr Create(::bash::domain::detail::WebScriptsInterfacePtr scripts);

  ::web::type::Command GetCommandName() override;

  ::web::type::JsonMessage Execute(const ::web::type::JsonMessage &message) override;

 private:
  ::bash::domain::detail::WebScriptsInterfacePtr scripts_;

  GetCommandsStatistics(::bash::domain::detail::WebScriptsInterfacePtr scripts);
};

}

}

}


#endif /* GET_COMMANDS_STATISTICS_H */
