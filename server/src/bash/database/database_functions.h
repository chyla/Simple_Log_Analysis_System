/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef DATABASE_FUNCTIONS_H
#define DATABASE_FUNCTIONS_H

#include "detail/database_functions_interface.h"

#include "src/bash/database/detail/raw_database_functions_interface.h"
#include "src/database/detail/general_database_functions_interface.h"
#include "src/database/detail/sqlite_wrapper_interface.h"

namespace bash
{

namespace database
{

class DatabaseFunctions;
typedef std::shared_ptr<DatabaseFunctions> DatabaseFunctionsPtr;

class DatabaseFunctions : public detail::DatabaseFunctionsInterface {
 public:
  virtual ~DatabaseFunctions() = default;

  static DatabaseFunctionsPtr Create(::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper,
                                     ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);

  static DatabaseFunctionsPtr Create(::bash::database::detail::RawDatabaseFunctionsInterfacePtr raw_database_functions,
                                     ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);

  void CreateTables() override;

  void AddSystemUser(type::UID uid) override;
  ::database::type::RowId GetSystemUserId(type::UID uid) override;

  void AddCommand(const ::bash::database::type::CommandName &command) override;
  ::database::type::RowId GetCommandId(const ::bash::database::type::CommandName &command) override;

  void AddLog(const ::type::BashLogEntry &log_entry) override;

 private:
  ::bash::database::detail::RawDatabaseFunctionsInterfacePtr raw_database_functions_;
  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;

  DatabaseFunctions(::bash::database::detail::RawDatabaseFunctionsInterfacePtr raw_database_functions,
                    ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);
};

}

}

#endif /* DATABASE_FUNCTIONS_H */
