/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef TESTS_MOCK_DATABASE_GENERAL_DATABASE_FUNCTIONS_H
#define TESTS_MOCK_DATABASE_GENERAL_DATABASE_FUNCTIONS_H

#include "src/database/detail/general_database_functions_interface.h"

#include <memory>
#include <gmock/gmock.h>

namespace mock
{

namespace database
{

class GeneralDatabaseFunctions : public ::database::detail::GeneralDatabaseFunctionsInterface {
 public:
  virtual ~GeneralDatabaseFunctions() = default;

  MOCK_METHOD0(CreateTables, void());

  MOCK_METHOD1(AddTime, void(const ::type::Time &t));

  MOCK_METHOD1(GetTimeId, ::database::type::RowId(const ::type::Time &t));

  MOCK_METHOD1(GetTimeById, const ::type::Time(::database::type::RowId id));
};

typedef std::shared_ptr<GeneralDatabaseFunctions> GeneralDatabaseFunctionsPtr;

}

}

#endif /* TESTS_MOCK_DATABASE_GENERAL_DATABASE_FUNCTIONS_H */
