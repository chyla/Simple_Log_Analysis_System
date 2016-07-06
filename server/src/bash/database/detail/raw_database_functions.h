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
  ::database::type::RowIds GetAllCommandsIds() override;
  ::bash::database::type::CommandName GetCommandNameById(::database::type::RowId id) override;

  void AddLog(const entity::Log &log) override;
  ::database::type::RowsCount CountCommandsForDailySystemStatistic(::database::type::RowId agent_name_id,
                                                                   ::database::type::RowId date_id,
                                                                   ::database::type::RowId command_id) override;

  void AddDailySystemStatistic(const entity::DailySystemStatistic &statistics) override;
  ::database::type::RowIds GetDateIdsWithoutCreatedDailySystemStatistic(::database::type::RowId agent_name_id) override;

  ::database::type::RowIds GetAgentIdsWithoutConfiguration() override;

  entity::AnomalyDetectionConfigurations GetAnomalyDetectionConfigurations() override;
  void RemoveAnomalyDetectionConfiguration(::database::type::RowId id) override;
  void AddAnomalyDetectionConfiguration(const entity::AnomalyDetectionConfiguration &configuration) override;

  void AddCommandStatistic(const entity::CommandStatistic &statistic) override;

  bool IsCommandStatisticExist(::database::type::RowId agent_name_id,
                               ::database::type::RowId command_id,
                               ::database::type::RowId begin_date_id,
                               ::database::type::RowId end_date_id) override;
  entity::CommandsStatistics GetCommandsStatistics(::database::type::RowId agent_name_id,
                                                   ::database::type::RowId begin_date_id,
                                                   ::database::type::RowId end_date_id) override;
  ::database::type::RowsCount CommandSummary(::database::type::RowId command_id,
                                             ::database::type::RowIds date_range_ids) override;
 private:
  ::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper_;

  RawDatabaseFunctions(::database::detail::SQLiteWrapperInterfacePtr sqlite_wrapper);
};

}

}

}

#endif /* RAW_DATABASE_FUNCTIONS_H */
