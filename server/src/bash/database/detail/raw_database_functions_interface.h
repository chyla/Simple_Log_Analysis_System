/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef RAW_DATABASE_FUNCTIONS_INTERFACE_H
#define RAW_DATABASE_FUNCTIONS_INTERFACE_H

#include "entity/daily_system_statistic.h"
#include "entity/log.h"
#include "entity/system_user.h"
#include "src/bash/database/type/command_name.h"

#include <memory>

namespace bash
{

namespace database
{

namespace detail
{

class RawDatabaseFunctionsInterface {
 public:
  virtual ~RawDatabaseFunctionsInterface() = default;

  virtual void CreateTables() = 0;

  virtual void AddSystemUser(const entity::SystemUser &system_user) = 0;
  virtual ::database::type::RowId GetSystemUserId(const entity::SystemUser &system_user) = 0;

  virtual void AddCommand(const ::bash::database::type::CommandName &command) = 0;
  virtual ::database::type::RowId GetCommandId(const ::bash::database::type::CommandName &command) = 0;
  virtual ::database::type::RowIds GetAllCommandsIds() = 0;

  virtual void AddLog(const entity::Log &log) = 0;
  virtual ::database::type::RowsCount CountCommandsForDailySystemStatistic(::database::type::RowId agent_name_id,
                                                                           ::database::type::RowId date_id,
                                                                           ::database::type::RowId command_id) = 0;

  virtual void AddDailySystemStatistic(const entity::DailySystemStatistic &statistics) = 0;
  virtual ::database::type::RowIds GetDateIdsWithoutCreatedDailySystemStatistic(::database::type::RowId agent_name_id) = 0;
};

typedef std::shared_ptr<RawDatabaseFunctionsInterface> RawDatabaseFunctionsInterfacePtr;

}

}

}

#endif /* RAW_DATABASE_FUNCTIONS_INTERFACE_H */
