/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef RAW_DATABASE_FUNCTIONS_H
#define RAW_DATABASE_FUNCTIONS_H

#include "raw_database_functions_interface.h"

#include "src/database/detail/sqlite_wrapper_interface.h"

namespace bash
{

namespace database
{

namespace detail
{

class RawDatabaseFunctions;
typedef std::shared_ptr<RawDatabaseFunctions> RawDatabaseFunctionsPtr;

class RawDatabaseFunctions : public RawDatabaseFunctionsInterface {
 public:
  virtual ~RawDatabaseFunctions() = default;

  static RawDatabaseFunctionsPtr Create(::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper);

  void CreateTables() override;

  void AddSystemUser(const entity::SystemUser &system_user) override;
  ::database::type::RowId GetSystemUserId(const entity::SystemUser &system_user) override;

  void AddCommand(const ::bash::database::type::CommandName &command) override;
  ::database::type::RowId GetCommandId(const ::bash::database::type::CommandName &command) override;

  void AddLog(const entity::Log &log) override;

 private:
  ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper_;

  RawDatabaseFunctions(::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper);
};

}

}

}

#endif /* RAW_DATABASE_FUNCTIONS_H */
