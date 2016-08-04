/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "web_command.h"

#include "src/bash/domain/detail/web_scripts_interface.h"

#include <memory>

namespace bash
{

namespace web
{

namespace detail
{

class GetAgentsWithExistingDailyUserStatistics;
typedef std::shared_ptr<GetAgentsWithExistingDailyUserStatistics> GetAgentsWithExistingDailyUserStatisticsPtr;

class GetAgentsWithExistingDailyUserStatistics : public WebCommand {
 public:
  virtual ~GetAgentsWithExistingDailyUserStatistics() = default;

  static GetAgentsWithExistingDailyUserStatisticsPtr Create(::bash::domain::detail::WebScriptsInterfacePtr scripts);

  ::web::type::Command GetCommandName() override;

  ::web::type::JsonMessage Execute(const ::web::type::JsonMessage &message) override;

 private:
  ::bash::domain::detail::WebScriptsInterfacePtr scripts_;

  GetAgentsWithExistingDailyUserStatistics(::bash::domain::detail::WebScriptsInterfacePtr scripts);
};

}

}

}
