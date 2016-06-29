/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "mailer.h"

#include "src/library/curl/curl_wrapper.h"
#include "src/library/curl/curl_guard.h"
#include "src/library/curl/slist_guard.h"
#include "src/library/curl/exception/curl_exception.h"

#include <boost/log/trivial.hpp>
#include <algorithm>

using namespace std;

namespace mailer
{

MailerPtr Mailer::Create(const ::program_options::type::Options &options) {
  return Create(options, ::library::curl::CurlWrapper::Create());
}

MailerPtr Mailer::Create(const ::program_options::type::Options &options,
                         ::library::curl::detail::CurlWrapperInterfacePtr curl_wrapper) {
  return MailerPtr(new Mailer(options, curl_wrapper));
}

void Mailer::Send(const detail::MailInterfacePtr mail) {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::Send: Function call";

  auto handler = curl_wrapper_->Init();
  ::library::curl::CurlGuard curl_guard(handler, curl_wrapper_);

  curl_wrapper_->SetOpt(handler, CURLOPT_USERNAME, options_.GetMailServerUser());
  curl_wrapper_->SetOpt(handler, CURLOPT_PASSWORD, options_.GetMailServerPassword());

  string url = GetUrl();
  curl_wrapper_->SetOpt(handler, CURLOPT_URL, url);

  if (options_.GetMailServerSecure() == ::program_options::type::SecurityOption::STARTTLS) {
    BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::Send: Using 'STARTTLS' command";
    curl_wrapper_->SetOpt(handler, CURLOPT_USE_SSL, (long) CURLUSESSL_ALL);
  }

  curl_wrapper_->SetOpt(handler, CURLOPT_MAIL_FROM, mail->GetFrom());

  curl_slist *recipients = GetRecipients(mail);
  ::library::curl::SListGuard slist_guard(recipients, curl_wrapper_);

  curl_wrapper_->SetOpt(handler, CURLOPT_MAIL_RCPT, recipients);

  type::RawMailText message_text = mail->Compose();

  UserData data;
  data.message_text = message_text.c_str();
  data.left = message_text.length();

  curl_wrapper_->SetOpt(handler, CURLOPT_READFUNCTION, payload_source);
  curl_wrapper_->SetOpt(handler, CURLOPT_READDATA, static_cast<void*> (&data));
  curl_wrapper_->SetOpt(handler, CURLOPT_UPLOAD, 1L);

  BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::Send: Sending";
  curl_wrapper_->Perform(handler);

  BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::Send: Done";
}

Mailer::Mailer(const ::program_options::type::Options &options,
               ::library::curl::detail::CurlWrapperInterfacePtr curl_wrapper) :
options_(options),
curl_wrapper_(curl_wrapper) {
}

const std::string Mailer::GetUrl() const {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::GetUrl: Function call";
  string url;

  if (options_.GetMailServerSecure() == ::program_options::type::SecurityOption::SSL) {
    BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::GetUrl: Using SSL";
    url = "smtps";
  }
  else {
    BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::GetUrl: Connection not secured with SSL";
    url = "smtp";
  }

  url = url + "://" + options_.GetMailServerAddress() + ":" + to_string(options_.GetMailServerPort());

  BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::GetUrl: Final url: " << url;

  return url;
}

curl_slist* Mailer::GetRecipients(const detail::MailInterfacePtr mail) const {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::GetRecipients: Function call";

  BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::GetRecipients: Creating curl_slist";
  curl_slist *recipients = nullptr, *old_recipients = nullptr;

  try {
    BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::GetRecipients: Adding 'To' recipients";
    for (const auto &r : mail->GetToRecipients()) {
      recipients = curl_wrapper_->SListAppend(old_recipients, r.c_str());
      old_recipients = recipients;
    }

    BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::GetRecipients: Adding 'Cc' recipients";
    for (const auto &r : mail->GetCcRecipients()) {
      recipients = curl_wrapper_->SListAppend(old_recipients, r.c_str());
      old_recipients = recipients;
    }
  }
  catch (::library::curl::exception::CurlException &ex) {
    BOOST_LOG_TRIVIAL(error) << "mailer::Mailer::GetRecipients: Exception: " << ex.what();
    curl_wrapper_->SListFreeAll(old_recipients);
    throw;
  }

  BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::GetRecipients: Returning recipients";
  return old_recipients;
}

size_t Mailer::payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
  BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::payload_source: Function call with size=" << size << "; nmemb=" << nmemb;

  UserData *data = static_cast<UserData*> (userp);
  char *dest = static_cast<char*> (ptr);

  if (size * nmemb < 1) {
    BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::payload_source: (size * nmemb < 1)";
    return 0;
  }

  if (data->left > 0) {
    BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::payload_source: (data->left > 0)";

    BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::payload_source: data->sent=" << data->sent << "; data->left=" << data->left;

    size_t to_send = min(nmemb, data->left);
    BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::payload_source: to_send=" << to_send;

    const char *begin = data->message_text + data->sent;
    copy(begin, begin + to_send, dest);

    data->sent += to_send;
    data->left -= to_send;

    BOOST_LOG_TRIVIAL(debug) << "mailer::Mailer::payload_source: data->sent=" << data->sent << "; data->left=" << data->left;

    return to_send;
  }
  else
    return 0;
}

}
