/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_DATABASE_GENERAL_DATABASE_FUNCTIONS_H
#define SRC_DATABASE_GENERAL_DATABASE_FUNCTIONS_H

#include "detail/general_database_functions_interface.h"

#include <memory>

#include "detail/sqlite_wrapper_interface.h"

namespace database
{

class GeneralDatabaseFunctions;
typedef std::shared_ptr<GeneralDatabaseFunctions> GeneralDatabaseFunctionsPtr;

class GeneralDatabaseFunctions : public detail::GeneralDatabaseFunctionsInterface {
 public:
  virtual ~GeneralDatabaseFunctions() = default;

  static GeneralDatabaseFunctionsPtr Create(detail::SQLiteWrapperInterfacePtr sqlite_wrapper);

  void CreateTables() override;

  void AddTime(const ::type::Time &t) override;
  ::database::type::RowId GetTimeId(const ::type::Time &t) override;
  const ::type::Time GetTimeById(::database::type::RowId id) override;

  void AddDate(const ::type::Date &date) override;
  ::database::type::RowId GetDateId(const ::type::Date &date) override;
  ::type::Date GetDateById(const ::database::type::RowId &id) override;

 private:
  GeneralDatabaseFunctions(detail::SQLiteWrapperInterfacePtr sqlite_wrapper);

  detail::SQLiteWrapperInterfacePtr sqlite_wrapper_;
};

}

#endif /* SRC_DATABASE_GENERAL_DATABASE_FUNCTIONS_H */
