/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_APACHE_DATABASE_DETAIL_DATABASE_FUNCTIONS_INTERFACE_H
#define SRC_APACHE_DATABASE_DETAIL_DATABASE_FUNCTIONS_INTERFACE_H

#include <memory>

#include <patlms/type/date.h>
#include <patlms/type/apache_log_entry.h>

#include "src/apache/type/apache_session_entry.h"
#include "src/database/type/rows_count.h"
#include "src/database/type/agent_name.h"
#include "src/database/type/virtualhost_name.h"

namespace apache
{

namespace database
{

namespace detail
{

class DatabaseFunctionsInterface {
 public:
  virtual ~DatabaseFunctionsInterface() = default;

  virtual ::database::type::RowsCount GetLogsCount(std::string agent_name, std::string virtualhost_name,
                                                   ::type::Date from, ::type::Date to) = 0;

  virtual ::type::ApacheLogs GetLogs(std::string agent_name, std::string virtualhost_name,
                                     ::type::Date from, ::type::Date to,
                                     unsigned limit, ::database::type::RowsCount offset) = 0;

  virtual bool AddSessionStatistics(const ::apache::type::ApacheSessions &sessions) = 0;

  virtual void MarkStatisticsAsCreatedFor(::type::Date date) = 0;

  virtual bool AreStatisticsCreatedFor(::type::Date date) = 0;

  virtual ::database::type::AgentNames GetAgentNames() = 0;

  virtual ::database::type::VirtualhostNames GetVirtualhostNames(::database::type::AgentName agent_name) = 0;
};

typedef std::shared_ptr<DatabaseFunctionsInterface> DatabaseFunctionsInterfacePtr;

}

}

}

#endif /* SRC_APACHE_DATABASE_DETAIL_DATABASE_FUNCTIONS_INTERFACE_H */
