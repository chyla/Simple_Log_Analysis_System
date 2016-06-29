/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "mail.h"

#include <boost/log/trivial.hpp>

namespace mailer
{

MailPtr Mail::Create() {
  return MailPtr(new Mail());
}

void Mail::SetDate(const ::type::Day &day, const ::type::Timestamp &timestamp, const ::mailer::type::TimeZone &time_zone) {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::SetDate: Function call";
  day_ = day;
  timestamp_ = timestamp;
  time_zone_ = time_zone;
}

void Mail::SetFrom(const ::mailer::type::MailAddress &from) {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::SetFrom: Function call";
  from_ = from;
}

void Mail::AddTo(const ::mailer::type::MailAddress &to) {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::AddTo: Function call";
  to_.push_back(to);
}

void Mail::AddCc(const ::mailer::type::MailAddress &cc) {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::AddCc: Function call";
  cc_.push_back(cc);
}

void Mail::SetSubject(const ::mailer::type::MailSubject &subject) {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::SetSubject: Function call";
  subject_ = subject;
}

void Mail::SetBody(const ::mailer::type::MailBody &body) {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::SetBody: Function call";
  body_ = body;
}

const ::mailer::type::RawMailText Mail::Compose() const {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::Compose: Function call";
  ::mailer::type::RawMailText raw_mail;

  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::Compose: Adding date: " << ::type::ToString(day_) << "; " << timestamp_.ToString() << "; " << time_zone_;
  raw_mail += "Date: " + ::type::ToString(day_) + ", " +
      std::to_string(timestamp_.GetDate().GetDay()) + " " +
      MonthToString(timestamp_.GetDate().GetMonth()) + " " +
      std::to_string(timestamp_.GetDate().GetYear()) + " " +
      timestamp_.GetTime().ToString() + " " +
      time_zone_ +
      "\r\n";

  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::Compose: Adding 'To' recipients";
  for (const auto &i : to_) {
    BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::Compose: 'Tc' recipient: " << i;
    raw_mail += "To: " + i + "\r\n";
  }

  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::Compose: Adding 'Cc' recipients";
  for (const auto &i : cc_) {
    BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::Compose: 'Cc' recipient: " << i;
    raw_mail += "Cc: " + i + "\r\n";
  }

  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::Compose: Adding 'From' field: " << from_;
  raw_mail += "From: " + from_ + "\r\n";

  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::Compose: Adding subject: " << subject_;
  raw_mail += "Subject: " + subject_ + "\r\n";

  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::Compose: Adding message body";
  raw_mail += "\r\n";

  raw_mail += body_;

  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::Compose: Message composed";

  return raw_mail;
}

const ::mailer::type::MailAddress &Mail::GetFrom() const {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::GetFrom: Function call";
  return from_;
}

const ::mailer::type::MailRecipients& Mail::GetToRecipients() const {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::GetToRecipients: Function call";
  return to_;
}

const ::mailer::type::MailRecipients& Mail::GetCcRecipients() const {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::GetCcRecipients: Function call";
  return cc_;
}

const std::string Mail::MonthToString(int month) const {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mail::MonthToString: Function call";

  switch (month) {
  case 1:
    return "Jan";

  case 2:
    return "Feb";

  case 3:
    return "Mar";

  case 4:
    return "Apr";

  case 5:
    return "May";

  case 6:
    return "Jun";

  case 7:
    return "Jul";

  case 8:
    return "Aug";

  case 9:
    return "Sep";

  case 10:
    return "Oct";

  case 11:
    return "Nov";

  case 12:
    return "Dec";
  }
}

}
