/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef MAILER_DETAIL_CURL_INTERFACE_H
#define MAILER_DETAIL_CURL_INTERFACE_H

#include <memory>
#include <curl/curl.h>

namespace library
{

namespace curl
{

namespace detail
{

class CurlInterface {
 public:
  virtual ~CurlInterface() = default;

  virtual CURL* EasyInit() = 0;

  virtual CURLcode EasySetOpt(CURL *handle, CURLoption option, void *parameter) = 0;

  virtual curl_slist* SListAppend(curl_slist *list, const char *string) = 0;

  virtual CURLcode EasyPerform(CURL *easy_handle) = 0;

  virtual const char* EasyStrError(CURLcode errornum) = 0;

  virtual void SListFreeAll(curl_slist *list) = 0;

  virtual void EasyCleanup(CURL *handle) = 0;
};

typedef std::shared_ptr<CurlInterface> CurlInterfacePtr;

}

}

}

#endif /* MAILER_DETAIL_CURL_INTERFACE_H */
