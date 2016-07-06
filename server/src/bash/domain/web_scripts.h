/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef WEB_SCRIPTS_H
#define WEB_SCRIPTS_H

#include "detail/scripts_interface.h"
#include "detail/web_scripts_interface.h"

namespace bash
{

namespace domain
{

class WebScripts;
typedef std::shared_ptr<WebScripts> WebScriptsPtr;

class WebScripts : public detail::WebScriptsInterface {
 public:
  virtual ~WebScripts() = default;

  static WebScriptsPtr Create(::bash::domain::detail::ScriptsInterfacePtr scripts);

  ::bash::domain::type::UnconfiguredAgents GetUnconfigurentAgents() override;

  ::bash::domain::type::AnomalyDetectionConfigurations GetAnomalyDetectionConfigurations() override;
  void RemoveAnomalyDetectionConfiguration(::database::type::RowId id) override;
  void SaveConfiguration(::database::type::RowId agent_name_id,
                         const ::type::Date &begin_date,
                         const ::type::Date &end_date) override;

  ::bash::domain::type::CommandsStatistics GetCommandsStatistics(::database::type::RowId agent_name_id,
                                                                 const ::type::Date &begin_date,
                                                                 const ::type::Date &end_date) override;

 private:
  ::bash::domain::detail::ScriptsInterfacePtr scripts_;

  WebScripts(::bash::domain::detail::ScriptsInterfacePtr scripts);
};

}

}

#endif /* WEB_SCRIPTS_H */
