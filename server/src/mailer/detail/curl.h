/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef MAILER_DETAIL_CURL_H
#define MAILER_DETAIL_CURL_H

#include "curl_interface.h"

namespace mailer
{

namespace detail
{

class Curl : public CurlInterface {
 public:
  virtual ~Curl() = default;

  CURL* EasyInit() override;

  CURLcode EasySetOpt(CURL *handle, CURLoption option, void *parameter) override;

  curl_slist* SListAppend(curl_slist *list, const char *string) override;

  CURLcode EasyPerform(CURL *easy_handle) override;

  const char* EasyStrError(CURLcode errornum) override;

  void SListFreeAll(curl_slist *list) override;

  void EasyCleanup(CURL *handle) override;
};

}

}

#endif /* MAILER_DETAIL_CURL_H */
