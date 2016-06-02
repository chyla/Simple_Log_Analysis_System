/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef TESTS_MOCK_APACHE_DATABASE_DATABASE_FUNCTIONS_H
#define TESTS_MOCK_APACHE_DATABASE_DATABASE_FUNCTIONS_H

#include <memory>
#include <gmock/gmock.h>

#include "src/apache/database/detail/database_functions_interface.h"

namespace mock
{

namespace apache
{

namespace database
{

class DatabaseFunctions;
typedef std::shared_ptr<DatabaseFunctions> DatabaseFunctionsPtr;

class DatabaseFunctions : public ::apache::database::detail::DatabaseFunctionsInterface {
 public:
  virtual ~DatabaseFunctions() = default;

  static DatabaseFunctionsPtr Create() {
    return std::make_shared<DatabaseFunctions>();
  }

  MOCK_METHOD4(GetLogsCount, ::database::type::RowsCount(std::string agent_name, std::string virtualhost_name,
                                                               ::type::Date from, ::type::Date to));

  MOCK_METHOD6(GetLogs, ::type::ApacheLogs(std::string agent_name, std::string virtualhost_name,
                                                 ::type::Date from, ::type::Date to,
                                                 unsigned limit, ::database::type::RowsCount offset));

  MOCK_METHOD1(AddSessionStatistics, bool(const ::apache::type::ApacheSessions &sessions));

  MOCK_METHOD1(MarkStatisticsAsCreatedFor, void(::type::Date date));

  MOCK_METHOD1(IsStatisticsCreatedFor, bool(::type::Date date));

  MOCK_METHOD0(GetAgentNames, ::database::type::AgentNames());

  MOCK_METHOD1(GetVirtualhostNames, ::database::type::VirtualhostNames(::database::type::AgentName agent_name));
};

}

}

}

#endif /* TESTS_MOCK_APACHE_DATABASE_DATABASE_FUNCTIONS_H */