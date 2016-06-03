/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_DATABASE_DETAIL_GENERAL_DATABASE_FUNCTIONS_INTERFACE_H
#define SRC_DATABASE_DETAIL_GENERAL_DATABASE_FUNCTIONS_INTERFACE_H

#include <patlms/type/time.h>

#include "src/database/type/row_id.h"

namespace database
{

namespace detail
{

class GeneralDatabaseFunctionsInterface {
 public:
  virtual ~GeneralDatabaseFunctionsInterface() = default;

  virtual void CreateTables() = 0;

  virtual void AddTime(const ::type::Time &t) = 0;
  
  virtual ::database::type::RowId GetTimeId(const ::type::Time &t) = 0;

  virtual const ::type::Time GetTimeById(::database::type::RowId id) = 0;
};

}

}

#endif /* SRC_DATABASE_DETAIL_GENERAL_DATABASE_FUNCTIONS_INTERFACE_H */
