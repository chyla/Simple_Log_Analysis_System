/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef DATABASE_FUNCTIONS_INTERFACE_H
#define DATABASE_FUNCTIONS_INTERFACE_H

#include <patlms/type/bash_log_entry.h>

#include "src/database/type/row_id.h"
#include "src/bash/database/type/uid.h"
#include "src/bash/database/type/command_name.h"
#include "src/bash/database/detail/entity/daily_system_statistic.h"

#include <memory>

namespace bash
{

namespace database
{

namespace detail
{

class DatabaseFunctionsInterface {
 public:
  virtual ~DatabaseFunctionsInterface() = default;

  virtual void CreateTables() = 0;

  virtual void AddSystemUser(type::UID uid) = 0;
  virtual ::database::type::RowId GetSystemUserId(type::UID uid) = 0;

  virtual void AddCommand(const ::bash::database::type::CommandName &command) = 0;
  virtual ::database::type::RowId GetCommandId(const ::bash::database::type::CommandName &command) = 0;
  virtual ::database::type::RowIds GetAllCommandsIds() = 0;

  virtual void AddLog(const ::type::BashLogEntry &log_entry) = 0;
  virtual ::database::type::RowsCount CountCommandsForDailySystemStatistic(::database::type::RowId agent_name_id,
                                                                           ::database::type::RowId date_id,
                                                                           ::database::type::RowId command_id) = 0;

  virtual void AddDailySystemStatistic(const entity::DailySystemStatistic &statistics) = 0;
  virtual ::database::type::RowIds GetDateIdsWithoutCreatedDailySystemStatistic(::database::type::RowId agent_name_id) = 0;
};

typedef std::shared_ptr<DatabaseFunctionsInterface> DatabaseFunctionsInterfacePtr;

}

}

}

#endif /* DATABASE_FUNCTIONS_INTERFACE_H */
