/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "mail.h"

namespace mailer
{

MailPtr Mail::Create() {
  return MailPtr(new Mail());
}

void Mail::SetDate(const ::type::Day &day, const ::type::Timestamp &timestamp, const ::mailer::type::TimeZone &time_zone) {
  day_ = day;
  timestamp_ = timestamp;
  time_zone_ = time_zone;
}

void Mail::SetFrom(const ::mailer::type::MailAddress &from) {
  from_ = from;
}

void Mail::AddTo(const ::mailer::type::MailAddress &to) {
  to_.push_back(to);
}

void Mail::AddCc(const ::mailer::type::MailAddress &cc) {
  cc_.push_back(cc);
}

void Mail::SetSubject(const ::mailer::type::MailSubject &subject) {
  subject_ = subject;
}

void Mail::SetBody(const ::mailer::type::MailBody &body) {
  body_ = body;
}

const ::mailer::type::RawMailText Mail::Compose() const {
  ::mailer::type::RawMailText raw_mail;

  raw_mail += "Date: " + ::type::ToString(day_) + ", " +
      std::to_string(timestamp_.GetDate().GetDay()) + " " +
      MonthToString(timestamp_.GetDate().GetMonth()) + " " +
      std::to_string(timestamp_.GetDate().GetYear()) + " " +
      timestamp_.GetTime().ToString() + " " +
      time_zone_ +
      "\r\n";

  for (const auto &i : to_)
    raw_mail += "To: " + i + "\r\n";

  for (const auto &i : cc_)
    raw_mail += "Cc: " + i + "\r\n";

  raw_mail += "Subject: " + subject_ + "\r\n";

  raw_mail += "\r\n";
  
  raw_mail += body_;
  
  return raw_mail;
}

const std::string Mail::MonthToString(int month) const {
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
