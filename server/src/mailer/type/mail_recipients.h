/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef MAILER_TYPE_MAIL_RECIPIENTS_H
#define MAILER_TYPE_MAIL_RECIPIENTS_H

#include "mail_address.h"

#include <vector>

namespace mailer
{

namespace type
{

typedef std::vector<MailAddress> MailRecipients;

}

}

#endif /* MAILER_TYPE_MAIL_RECIPIENTS_H */
