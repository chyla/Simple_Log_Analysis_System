/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef MAILER_MAIL_H
#define MAILER_MAIL_H

#include "detail/mail_interface.h"
#include <vector>

namespace mailer
{

class Mail;
typedef std::shared_ptr<Mail> MailPtr;

class Mail : public detail::MailInterface {
 public:
  virtual ~Mail() = default;

  static MailPtr Create();

  void SetDate(const ::type::Day &day, const ::type::Timestamp &timestamp, const ::mailer::type::TimeZone &time_zone) override;

  void SetFrom(const ::mailer::type::MailAddress &from) override;
  void AddTo(const ::mailer::type::MailAddress &to) override;
  void AddCc(const ::mailer::type::MailAddress &cc) override;

  void SetSubject(const ::mailer::type::MailSubject &subject) override;
  void SetBody(const ::mailer::type::MailBody &body) override;

  const ::mailer::type::RawMailText Compose() const override;

 private:
  ::type::Day day_;
  ::type::Timestamp timestamp_;
  ::mailer::type::TimeZone time_zone_;

  ::mailer::type::MailAddress from_;
  std::vector<::mailer::type::MailAddress> to_;
  std::vector<::mailer::type::MailAddress> cc_;

  ::mailer::type::MailSubject subject_;
  ::mailer::type::MailBody body_;

  Mail() = default;
  const std::string MonthToString(int month) const;
};

}

#endif /* MAILER_MAIL_H */
