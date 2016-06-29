/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef MAILER_DETAIL_CURL_H
#define MAILER_DETAIL_CURL_H

#include "detail/curl_interface.h"

namespace library
{

namespace curl
{

class Curl;
typedef std::shared_ptr<Curl> CurlPtr;

class Curl : public detail::CurlInterface {
 public:
  virtual ~Curl() = default;

  static CurlPtr Create();

  CURL* EasyInit() override;

  CURLcode EasySetOpt(CURL *handle, CURLoption option, void *parameter) override;

  CURLcode EasySetOpt(CURL *handle, CURLoption option, size_t(*f)(void*, size_t, size_t, void*)) override;

  CURLcode EasySetOpt(CURL *handle, CURLoption option, const std::string &parameter) override;

  CURLcode EasySetOpt(CURL *handle, CURLoption option, const char *parameter) override;

  CURLcode EasySetOpt(CURL *handle, CURLoption option, long parameter) override;

  CURLcode EasySetOpt(CURL *handle, CURLoption option, curl_slist *parameter) override;

  curl_slist* SListAppend(curl_slist *list, const char *string) override;

  CURLcode EasyPerform(CURL *easy_handle) override;

  const char* EasyStrError(CURLcode errornum) override;

  void SListFreeAll(curl_slist *list) override;

  void EasyCleanup(CURL *handle) override;
};


}

}

#endif /* MAILER_DETAIL_CURL_H */
