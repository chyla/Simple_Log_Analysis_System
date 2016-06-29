/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef MAILER_DETAIL_MAIL_INTERFACE_H
#define MAILER_DETAIL_MAIL_INTERFACE_H

#include <string>
#include <memory>

#include <patlms/type/timestamp.h>
#include <patlms/type/day.h>

#include "src/mailer/type/time_zone.h"
#include "src/mailer/type/mail_address.h"
#include "src/mailer/type/raw_mail_text.h"
#include "src/mailer/type/mail_subject.h"
#include "src/mailer/type/mail_body.h"
#include "src/mailer/type/mail_recipients.h"

namespace mailer
{

namespace detail
{

class MailInterface {
 public:
  virtual ~MailInterface() = default;

  virtual void SetDate(const ::type::Day &day, const ::type::Timestamp &timestamp, const ::mailer::type::TimeZone &time_zone) = 0;

  virtual void SetFrom(const ::mailer::type::MailAddress &from) = 0;
  virtual void AddTo(const ::mailer::type::MailAddress &to) = 0;
  virtual void AddCc(const ::mailer::type::MailAddress &cc) = 0;

  virtual void SetSubject(const ::mailer::type::MailSubject &subject) = 0;
  virtual void SetBody(const ::mailer::type::MailBody &body) = 0;

  virtual const ::mailer::type::MailAddress &GetFrom() const = 0;
  virtual const ::mailer::type::MailRecipients& GetToRecipients() const = 0;
  virtual const ::mailer::type::MailRecipients& GetCcRecipients() const = 0;

  virtual const ::mailer::type::RawMailText Compose() const = 0;
};

typedef std::shared_ptr<MailInterface> MailInterfacePtr;

}

}

#endif /* MAIL_INTERFACE_H */
