/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SRC_DATABASE_DETAIL_GENERAL_DATABASE_FUNCTIONS_INTERFACE_H
#define SRC_DATABASE_DETAIL_GENERAL_DATABASE_FUNCTIONS_INTERFACE_H

#include <memory>

#include <patlms/type/time.h>
#include <patlms/type/date.h>

#include "src/database/type/agent_name.h"
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
  virtual ::database::type::RowId AddAndGetTimeId(const ::type::Time &time) = 0;
  virtual ::database::type::RowId GetTimeId(const ::type::Time &t) = 0;
  virtual const ::type::Time GetTimeById(::database::type::RowId id) = 0;

  virtual void AddDate(const ::type::Date &date) = 0;
  virtual ::database::type::RowId AddAndGetDateId(const ::type::Date &date) = 0;
  virtual ::database::type::RowId GetDateId(const ::type::Date &date) = 0;
  virtual ::type::Date GetDateById(const ::database::type::RowId &id) = 0;

  virtual void AddAgentName(const std::string &name) = 0;
  virtual ::database::type::AgentNames GetAgentNames() = 0;
  virtual ::database::type::RowIds GetAgentsIds() = 0;
  virtual ::database::type::RowId AddAndGetAgentNameId(const std::string &name) = 0;
  virtual ::database::type::RowId GetAgentNameId(const std::string &name) = 0;
  virtual std::string GetAgentNameById(const ::database::type::RowId &id) = 0;
};

typedef std::shared_ptr<GeneralDatabaseFunctionsInterface> GeneralDatabaseFunctionsInterfacePtr;

}

}

#endif /* SRC_DATABASE_DETAIL_GENERAL_DATABASE_FUNCTIONS_INTERFACE_H */
