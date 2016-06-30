/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef NOTIFIER_TYPE_NOTIFIER_MESSAGES_H
#define NOTIFIER_TYPE_NOTIFIER_MESSAGES_H

#include "notifier_message_interface.h"

#include <list>

namespace notifier
{

namespace type
{

typedef std::list<NotifierMessageInterfacePtr> NotifierMessages;

}

}

#endif /* NOTIFIER_TYPE_NOTIFIER_MESSAGES_H */
