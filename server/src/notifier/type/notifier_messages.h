/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "notifier_message_interface.h"

#include <list>

namespace notifier
{

namespace type
{

typedef std::list<NotifierMessageInterfacePtr> NotifierMessages;

}

}
