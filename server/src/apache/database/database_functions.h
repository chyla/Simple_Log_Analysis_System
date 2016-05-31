/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_APACHE_DATABASE_DATABASE_FUNCTIONS_H
#define SRC_APACHE_DATABASE_DATABASE_FUNCTIONS_H

#include <memory>

#include "detail/database_functions_interface.h"

#include "src/database/detail/sqlite_wrapper_interface.h"
// will be removed in the future
#include "src/database/database.h"

namespace apache
{

namespace database
{

class DatabaseFunctions;
typedef std::shared_ptr<DatabaseFunctions> DatabaseFunctionsPtr;

class DatabaseFunctions : public detail::DatabaseFunctionsInterface {
 public:
  virtual ~DatabaseFunctions() = default;

  static DatabaseFunctionsPtr Create(::database::DatabasePtr db,
                                     ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper);

  ::database::type::RowsCount GetApacheLogsCount(std::string agent_name, std::string virtualhost_name,
                                                 ::type::Date from, ::type::Date to) override;

  ::type::ApacheLogs GetApacheLogs(std::string agent_name, std::string virtualhost_name,
                                   ::type::Date from, ::type::Date to,
                                   unsigned limit, ::database::type::RowsCount offset) override;

  bool AddApacheSessionStatistics(const ::apache::type::ApacheSessions &sessions) override;

  void MarkApacheStatisticsAsCreatedFor(::type::Date date) override;

  bool IsApacheStatisticsCreatedFor(::type::Date date) override;

 private:
  ::database::DatabasePtr db_;
  ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper_;

  DatabaseFunctions(::database::DatabasePtr db,
                    ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper);
};

}

}

#endif /* SRC_APACHE_DATABASE_DATABASE_FUNCTIONS_H */
