/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include <slas/type/timestamp.h>
#include <boost/log/trivial.hpp>
#include <slas/type/exception/exception.h>

#include "notifier.h"

#include "detail/system.h"
#include "src/mailer/mail.h"
#include "src/mailer/mailer.h"

using namespace std;

namespace notifier
{

NotifierPtr Notifier::Create(const ::program_options::type::Options &options) {
  return Create(options,
                ::mailer::Mailer::Create(options),
                detail::System::Create());
}

NotifierPtr Notifier::Create(const ::program_options::type::Options &options,
                             ::mailer::detail::MailerInterfacePtr mailer,
                             detail::SystemInterfacePtr system) {
  return NotifierPtr(new Notifier(options, mailer, system));
}

void Notifier::Loop() {
  BOOST_LOG_TRIVIAL(debug) << "notifier::Notifier::Loop: Function call";
  running_ = true;

  while (running_) {
    BOOST_LOG_TRIVIAL(debug) << "notifier::Notifier::Loop: Loop is running";

    try {
      if (IsMessageAvailable())
        SendMailNotification();
    }
    catch (::interface::Exception &ex) {
      BOOST_LOG_TRIVIAL(error) << "notifier::Notifier::Loop: Exception catched: " << ex.what();
      BOOST_LOG_TRIVIAL(info) << "notifier::Notifier::Loop: Continuing...";
    }

    system_->Sleep(1);
  }

  BOOST_LOG_TRIVIAL(debug) << "notifier::Notifier::Loop: Done";
}

void Notifier::StopLoop() {
  BOOST_LOG_TRIVIAL(debug) << "notifier::Notifier::StopLoop: Function call";
  running_ = false;
}

bool Notifier::IsRunning() const {
  return running_;
}

void Notifier::AddMessages(::notifier::type::NotifierMessages messages) {
  BOOST_LOG_TRIVIAL(debug) << "notifier::Notifier::AddMessages: Function call";

  lock_guard<mutex> guard(messages_mutex_);

  for (auto msg : messages)
    messages_.push_back(msg);
}

Notifier::Notifier(const ::program_options::type::Options &options,
                   ::mailer::detail::MailerInterfacePtr mailer,
                   detail::SystemInterfacePtr system) :
options_(options),
mailer_(mailer),
system_(system),
running_(false) {
}

::notifier::type::NotifierMessageInterfacePtr Notifier::GetMessage() {
  BOOST_LOG_TRIVIAL(debug) << "notifier::Notifier::GetMessage: Function call";

  lock_guard<mutex> guard(messages_mutex_);

  auto message = messages_.front();
  messages_.pop_front();

  return message;
}

bool Notifier::IsMessageAvailable() {
  lock_guard<mutex> guard(messages_mutex_);

  return !messages_.empty();
}

void Notifier::SendMailNotification() {
  BOOST_LOG_TRIVIAL(debug) << "notifier::Notifier::SendMailNotification: Function call";

  string mail_body = "Found new anomalies.";

  BOOST_LOG_TRIVIAL(debug) << "notifier::Notifier::SendMailNotification: Creating message body";
  while (IsMessageAvailable()) {
    auto message = GetMessage();

    mail_body += "\r\n\r\n"
        "Module: " + message->GetModuleName() + "\r\n"
        "================================================================================\r\n";
    mail_body += message->GetDetectionResults();
  }

  BOOST_LOG_TRIVIAL(debug) << "notifier::Notifier::SendMailNotification: Preparing message";
  mailer::MailPtr mail = mailer::Mail::Create();
  mail->AddTo(options_.GetMailTo());
  mail->SetFrom(options_.GetMailFrom());
  mail->SetSubject("Found new anomalies");
  mail->SetBody(mail_body);

  time_t t = system_->Time();
  tm *gmt = system_->GMTime(&t);

  mail->SetDate(::type::ToDay(gmt->tm_wday),
                ::type::Timestamp::Create(gmt->tm_hour, gmt->tm_min, gmt->tm_sec % 60,
                                          gmt->tm_mday, gmt->tm_mon + 1, gmt->tm_year + 1900),
                "+0000");

  BOOST_LOG_TRIVIAL(debug) << "notifier::Notifier::SendMailNotification: Sending message";
  mailer::MailerPtr sender = mailer::Mailer::Create(options_);
  sender->Send(mail);
}

}
