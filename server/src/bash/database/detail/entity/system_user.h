/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef SYSTEM_USER_H
#define SYSTEM_USER_H

#include "src/bash/database/type/uid.h"

namespace bash
{

namespace database
{

namespace detail
{

namespace entity
{

struct SystemUser {
  ::bash::database::type::UID uid;
};

}

}

}

}

#endif /* SYSTEM_USER_H */
