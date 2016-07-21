/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef GET_DAILY_USER_STATISTICS_FROM_CONFIGURATION_H
#define GET_DAILY_USER_STATISTICS_FROM_CONFIGURATION_H

#include "web_command.h"

#include "src/bash/domain/detail/web_scripts_interface.h"

#include <memory>

namespace bash
{

namespace web
{

namespace detail
{

class GetDailyUserStatisticsFromConfiguration;
typedef std::shared_ptr<GetDailyUserStatisticsFromConfiguration> GetDailyUserStatisticsFromConfigurationPtr;

class GetDailyUserStatisticsFromConfiguration : public WebCommand {
 public:
  virtual ~GetDailyUserStatisticsFromConfiguration() = default;

  static GetDailyUserStatisticsFromConfigurationPtr Create(::bash::domain::detail::WebScriptsInterfacePtr scripts);

  ::web::type::Command GetCommandName() override;

  ::web::type::JsonMessage Execute(const ::web::type::JsonMessage &message) override;

 private:
  ::bash::domain::detail::WebScriptsInterfacePtr scripts_;

  GetDailyUserStatisticsFromConfiguration(::bash::domain::detail::WebScriptsInterfacePtr scripts);
};

}

}

}

#endif /* GET_DAILY_USER_STATISTICS_FROM_CONFIGURATION_H */
