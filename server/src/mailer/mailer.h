/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#pragma once

#include "detail/mailer_interface.h"
#include "src/program_options/type/options.h"
#include "src/library/curl/detail/curl_wrapper_interface.h"

namespace mailer
{

class Mailer;
typedef std::shared_ptr<Mailer> MailerPtr;

class Mailer : public detail::MailerInterface {
 public:
  virtual ~Mailer() = default;

  static MailerPtr Create(const ::program_options::type::Options &options);
  static MailerPtr Create(const ::program_options::type::Options &options,
                          ::library::curl::detail::CurlWrapperInterfacePtr curl_wrapper);

  void Send(const detail::MailInterfacePtr mail) override;

 private:

  struct UserData {

    UserData() :
    message_text(nullptr),
    left(0),
    sent(0) {
    }

    const char *message_text;
    size_t left;
    size_t sent;
  };

  Mailer(const ::program_options::type::Options &options,
         ::library::curl::detail::CurlWrapperInterfacePtr curl_wrapper);

  const std::string GetUrl() const;
  curl_slist* GetRecipients(const detail::MailInterfacePtr mail) const;

  static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);

  const ::program_options::type::Options &options_;

  ::library::curl::detail::CurlWrapperInterfacePtr curl_wrapper_;
};

}
