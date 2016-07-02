/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SCRIPTS_H
#define SCRIPTS_H

#include "detail/scripts_interface.h"
#include "src/bash/database/detail/database_functions_interface.h"
#include "src/database/detail/general_database_functions_interface.h"

namespace bash
{

namespace domain
{

class Scripts;
typedef std::shared_ptr<Scripts> ScriptsPtr;

class Scripts : public detail::ScriptsInterface {
 public:
  virtual ~Scripts() = default;

  static ScriptsPtr Create(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
                           ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);

  void AddLog(const ::type::BashLogEntry &log_entry) override;

 private:
  ::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions_;
  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;

  Scripts(::bash::database::detail::DatabaseFunctionsInterfacePtr database_functions,
          ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions);
};

}

}

#endif /* SCRIPTS_H */
