/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef LOG_H
#define LOG_H

#include "src/database/type/row_id.h"
#include "src/bash/database/type/command_name.h"

namespace bash
{

namespace database
{

namespace detail
{

namespace entity
{

struct Log {
  ::database::type::RowId agent_name_id;
  ::database::type::RowId time_id;
  ::database::type::RowId date_id;
  ::database::type::RowId user_id;
  ::bash::database::type::CommandName command;
};

}

}

}

}

#endif /* LOG_H */
